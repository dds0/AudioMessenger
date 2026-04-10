
local enum_type = {
    [0] = "Request",
    [1] = "Reply",
    [2] = "Status",
    [3] = "ReqNoReply"
}

local enum_code = {
    [0] = "Status",
    [1] = "StartCall",
    [2] = "EndCall",
    [3] = "DenyCall",
    [4] = "Voice"
}

local enum_codec = {
    [0] = "pcm"
}

local protocol = Proto("amcdp","AMCDP protocol")

-- Header
protocol.fields.header_prefix = ProtoField.uint32("amcdp.header.prefix", "prefix", base.HEX)
protocol.fields.header_device = ProtoField.uint8("amcdp.header.device", "device", base.HEX)
protocol.fields.header_type = ProtoField.uint8("amcdp.header.type", "type", base.HEX, enum_type)
protocol.fields.header_code = ProtoField.uint8("amcdp.header.code", "code", base.HEX, enum_code)
-- Voice
protocol.fields.voice_codec = ProtoField.uint8("amcdp.voice.codec", "codec", base.HEX, enum_codec)
protocol.fields.voice_duration = ProtoField.uint8("amcdp.voice.duration", "duration", base.DEC)
protocol.fields.voice_payload = ProtoField.uint8("amcdp.voice.payload", "dataLen", base.HEX)

function protocol.dissector(buffer, pinfo, tree)

    local offset = 0

    if buffer:len() < 7 then
        return
    end

    local protoId = buffer(offset, 4):le_uint()
    if protoId ~= 0xFEA0DDCC then
        return
    end
    
    local subtree = tree:add(protocol, buffer(), "AMCDP protocol")

    subtree:add(protocol.fields.header_prefix, buffer(offset, 4):le_uint())
    offset = offset + 4
    
    subtree:add(protocol.fields.header_device, buffer(offset, 1):le_uint())
    offset = offset + 1

    local type = buffer(offset, 1):le_uint()    
    local type_str = enum_type[type] or "Unknown"
    
    subtree:add(protocol.fields.header_type, buffer(offset, 1):le_uint())
    offset = offset + 1
    
    local code = buffer(offset, 1):le_uint()  
    local code_str = enum_code[code] or "Unknown"
    
    subtree:add(protocol.fields.header_code, buffer(offset, 1):le_uint())
    offset = offset + 1

    if code == 4 then -- Voice

	subtree = subtree:add(protocol, buffer(), "VOICE")
	
        if buffer:len() < offset + 2 then
            return
        end

        local codec = buffer(offset, 1):uint()
        subtree:add(protocol.fields.voice_codec, buffer(offset, 1))
        offset = offset + 1

        local duration = buffer(offset, 1):uint()
        subtree:add(protocol.fields.voice_duration, buffer(offset, 1))
        offset = offset + 1

	local dataLen = duration * 16
	i=0
    	while(i < dataLen)
    	do
            tr = subtree:add("payload" .. "[" .. tostring(i) .. "]")
            tr:add_le(protocol.fields.voice_payload, buffer(offset + i, 1))
            i = i + 1
    	end

        --local payload_len = buffer:len() - offset
        --if payload_len > 0 then
        --    subtree:add(protocol.fields.voice_payload, buffer(offset, payload_len))
        --end
    end

    pinfo.cols.info = pinfo.src_port.." → "..pinfo.dst_port.." Len="..
        buffer:len().." "..type_str.." "..code_str
end

local udp_table = DissectorTable.get("udp.port")
udp_table:add(12345, protocol)
