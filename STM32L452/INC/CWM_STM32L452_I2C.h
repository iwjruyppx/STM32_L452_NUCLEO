
#ifndef __CWM_STM32L452_I2C_H
#define __CWM_STM32L452_I2C_H

typedef enum {
    CWM_I2C1 = 0,
    CWM_I2C2,
    CWM_I2C3,
}CWM_I2C_e;

typedef struct {
    I2C_HandleTypeDef  *handle;
    void (*init)(void);
    void (*deInit)(void);
    void (*mspInit)(I2C_HandleTypeDef *);
    void (*mspDeInit)(I2C_HandleTypeDef *);

    int (*Write)(uint16_t, uint8_t, uint8_t *, uint16_t);
    int (*Read)(uint16_t, uint8_t, uint8_t *, uint16_t);
    int (*buffWrite)(uint16_t, uint8_t *, uint16_t);
    int (*buffRead)(uint16_t, uint8_t *, uint16_t);
    int (*singleRegWrite)(uint16_t, uint8_t, uint8_t);
    int (*singleRegRead)(uint16_t, uint8_t, uint8_t);
} I2cClass_t, *pI2cClass_t;

void CWM_I2C_INIT(void);
pI2cClass_t CWM_GET_I2C_HANDLE(CWM_I2C_e dev);

#endif /* __CWM_STM32L452_I2C_H */
