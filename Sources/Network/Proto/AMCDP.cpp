#include "AMCDP.h"
#include <QDebug>
#include <cstring>

using namespace AMCDPSPace;

AMCDP::AMCDP(QObject *parent) : QObject(parent) {}

void AMCDP::setDeviceId(uint8_t deviceId)
{
    _deviceId = deviceId;
}

void AMCDP::send_reqStartCall()
{
    emit send(serializeHeader(TypeCommand::Request, CodeCommand::StartCall));
}

void AMCDP::send_replyStartCall()
{
    emit send(serializeHeader(TypeCommand::Reply, CodeCommand::StartCall));
}

void AMCDP::send_statusActivity()
{
    emit send(serializeHeader(TypeCommand::Status, CodeCommand::Status));
}

void AMCDP::send_reqNoReplyEndCall()
{
    emit send(serializeHeader(TypeCommand::ReqNoReply, CodeCommand::EndCall));
}

void AMCDP::send_reqNoReplyDenyCall()
{
    emit send(serializeHeader(TypeCommand::ReqNoReply, CodeCommand::DenyCall));
}

void AMCDP::send_reqNoReplyVoice(const Voice &voice)
{
    auto bytes = serializeHeader(TypeCommand::ReqNoReply, CodeCommand::Voice);

    bytes.append(static_cast<uint8_t>(voice.codec));
    bytes.append(voice.duration);
    bytes.append(voice.payload);

    emit send(bytes);
}

void AMCDP::parse(QByteArray bytes)
{
    bool correctness = true;
    auto header = deserializeHeader(bytes, correctness);

    if (!correctness) {
        return;
    }
    bytes.remove(0, sizeof(header));

    switch (header.type)
    {
    case TypeCommand::Request:
        {
            if (header.command == CodeCommand::StartCall)
            {
                emit receive_reqStartCall();
                return;
            }
        }
    break;
    case TypeCommand::Reply:
        {
            if (header.command == CodeCommand::StartCall)
            {
                emit receive_replyStartCall();
                return;
            }
        }
    break;
    case TypeCommand::Status:
        {
            if (header.command == CodeCommand::Status)
            {
                emit receive_statusActivity();
                return;
            }
        }
    break;
    case TypeCommand::ReqNoReply:
        {
            if (header.command == CodeCommand::EndCall)
            {
                emit receive_reqNoReplyEndCall();
                return;
            }

            if (header.command == CodeCommand::DenyCall)
            {
                emit receive_reqNoReplyDenyCall();
                return;
            }

            if (header.command == CodeCommand::Voice)
            {
                auto voice = parseVoice(bytes);
                if (voice) {
                    emit receive_reqNoReplyVoice(voice.value());
                }
                return;
            }
        }
    break;
        default:
            break;
    }
}

std::optional<Voice> AMCDP::parseVoice(QByteArray &bytes)
{
    Voice voice;

    int sizeBytes;

    sizeBytes = sizeof(uint8_t(Voice::codec));
    if (sizeBytes > bytes.size()) {
        return {};
    }

    voice.codec = AC::Codec(bytes.left(sizeBytes).toUInt());
    bytes.remove(0, sizeBytes);

    if (voice.codec != AC::Codec::Pcm) {
        return {};
    }

    sizeBytes = sizeof(voice.duration);
    if (sizeBytes > bytes.size()) {
        return {};
    }

    voice.duration = bytes[0];
    bytes.remove(0, sizeBytes);

    sizeBytes = AC::BytesPerMillisecond * voice.duration;
    if (sizeBytes > bytes.size()) {
        return {};
    }

    voice.payload = bytes.left(sizeBytes);
    bytes.remove(0, sizeBytes);

    return voice;
}

QByteArray AMCDP::serializeHeader(TypeCommand type, CodeCommand command)
{
    QByteArray header;
    header.reserve(sizeof(Header));

    header.append(reinterpret_cast<const char*>(&prefix), sizeof(prefix));
    header.append(_deviceId);
    header.append(uint8_t(type));
    header.append(uint8_t(command));

    return header;
}

Header AMCDP::deserializeHeader(QByteArray& bytes, bool &state)
{
    Header header{};

    if (uint(bytes.size()) < sizeof(Header))  {
        state = false; return header;
    }
    std::memcpy(&header, bytes.data(), sizeof(Header));

    if (header.protoId != prefix || header.deviceId == _deviceId) {
        state = false; return header;
    }
    return header;
}
