HEADERS += \
    $$PWD/vad/include/webrtc_vad.h

SOURCES += \
    $$PWD/other/complex_bit_reverse.c       \
    $$PWD/other/complex_fft.c               \
    $$PWD/other/copy_set_operations.c       \
    $$PWD/other/cross_correlation.c         \
    $$PWD/other/delay_estimator.c           \
    $$PWD/other/delay_estimator_wrapper.c   \
    $$PWD/other/division_operations.c       \
    $$PWD/other/dot_product_with_scale.c    \
    $$PWD/other/downsample_fast.c           \
    $$PWD/other/energy.c                    \
    $$PWD/other/fft4g.c                     \
    $$PWD/other/float_util.c                \
    $$PWD/other/get_scaling_square.c        \
    $$PWD/other/high_pass_filter.c          \
    $$PWD/other/min_max_operations.c        \
    $$PWD/other/randomization_functions.c   \
    $$PWD/other/real_fft.c                  \
    $$PWD/other/resample_48khz.c            \
    $$PWD/other/resample_by_2.c             \
    $$PWD/other/resample_by_2_internal.c    \
    $$PWD/other/resample_fractional.c       \
    $$PWD/other/ring_buffer.c               \
    $$PWD/other/splitting_filter.c          \
    $$PWD/other/spl_init.c                  \
    $$PWD/other/spl_sqrt.c                  \
    $$PWD/other/spl_sqrt_floor.c            \
    $$PWD/other/vector_scaling_operations.c \
    $$PWD/vad/vad_core.c                    \
    $$PWD/vad/vad_filterbank.c              \
    $$PWD/vad/vad_gmm.c                     \
    $$PWD/vad/vad_sp.c                      \
    $$PWD/vad/webrtc_vad.c

