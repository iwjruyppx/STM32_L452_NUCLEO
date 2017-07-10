
#ifndef __CWM_SCREEN_H
#define __CWM_SCREEN_H

typedef struct {
    CWM_INFO_CALLBACK info;
    union { //CWM_SCREEN_WRITE_AUTO_NEW_LINE
        char *string;
    };
}CWM_SCREEN_CMD_t, *pCWM_SCREEN_CMD_t;

int CWM_SCREEN_INIT(void);
int CWM_SCREEN_CMD(int cmd, void *p);

int CWM_SCREEN_WRITE_AUTO_NEW_LINE(char * data);

#endif /* __CWM_SCREEN_H */
