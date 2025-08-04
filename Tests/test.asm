start:  CLS
        ld  I, 140
        ld  V0, 12
        ld  V1, 8
        DRW V0, V1, 5
        ADD  V0, 9
        ld  I, i_data
        DRW V0, V1, 5
        ADD  V0, 9
        ld  I, 130
        DRW V0, V1, 5
        ADD  V0, 9
        ld  I, 80
        DRW V0, V1, 5
        ADD  V0, 14
        ADD V1, 10
        ld  I, sm_data
        DRW V0, V1, 5
loop:   JP loop
i_data: db #f0, #60, #60, #60
        db #f0
sm_data: db #00, #90, #00, #90
         db #f0
