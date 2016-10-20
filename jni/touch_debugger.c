/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/mman.h>
#include <errno.h>
#include <android/log.h>

#define TOUCH_IOCTL_BASE            		0xbc
#define TOUCH_IOCTL_GET_DEBUGMSG_STATE		_IOW(TOUCH_IOCTL_BASE,0,int)
#define TOUCH_IOCTL_SET_DEBUGMSG_STATE		_IOW(TOUCH_IOCTL_BASE,1,int)
#define TOUCH_IOCTL_GET_CHIP_REVISION		_IOW(TOUCH_IOCTL_BASE,2,int)
#define TOUCH_IOCTL_GET_FW_VERSION		_IOW(TOUCH_IOCTL_BASE,3,int)
#define TOUCH_IOCTL_GET_REG_DATA_VERSION	_IOW(TOUCH_IOCTL_BASE,4,int)
#define TOUCH_IOCTL_VARIFY_UPGRADE_SIZE		_IOW(TOUCH_IOCTL_BASE,5,int)
#define TOUCH_IOCTL_VARIFY_UPGRADE_DATA		_IOW(TOUCH_IOCTL_BASE,6,int)
#define TOUCH_IOCTL_START_UPGRADE		_IOW(TOUCH_IOCTL_BASE,7,int)
#define TOUCH_IOCTL_GET_X_NODE_NUM		_IOW(TOUCH_IOCTL_BASE,8,int)
#define TOUCH_IOCTL_GET_Y_NODE_NUM		_IOW(TOUCH_IOCTL_BASE,9,int)
#define TOUCH_IOCTL_GET_TOTAL_NODE_NUM		_IOW(TOUCH_IOCTL_BASE,10,int)
#define TOUCH_IOCTL_SET_RAW_DATA_MODE		_IOW(TOUCH_IOCTL_BASE,11,int)
#define TOUCH_IOCTL_GET_RAW_DATA		_IOW(TOUCH_IOCTL_BASE,12,int)
#define TOUCH_IOCTL_GET_X_RESOLUTION		_IOW(TOUCH_IOCTL_BASE,13,int)
#define TOUCH_IOCTL_GET_Y_RESOLUTION		_IOW(TOUCH_IOCTL_BASE,14,int)
#define TOUCH_IOCTL_HW_CALIBRAION		_IOW(TOUCH_IOCTL_BASE,15,int)
#define TOUCH_IOCTL_GET_REG			_IOW(TOUCH_IOCTL_BASE,16,int)
#define TOUCH_IOCTL_SET_REG			_IOW(TOUCH_IOCTL_BASE,17,int)
#define TOUCH_IOCTL_SEND_SAVE_STATUS		_IOW(TOUCH_IOCTL_BASE,18,int)
#define TOUCH_IOCTL_DONOT_TOUCH_EVENT		_IOW(TOUCH_IOCTL_BASE,19,int)


#define	TOUCH_NORMAL_MODE				48

#define	MISC_TOUCH_DEV	"/dev/zinitix_touch_misc"

typedef struct 
{
	int	sz;	
	char	*buf;	
} _raw_ioctl;

typedef struct 
{
	int	addr;	
	int	*val;	
} _reg_ioctl;


jint
Java_com_mitac_touch_TouchDebugger_GetChipRevision( JNIEnv* env, jobject thiz )
{
	int fd,ret;
	char buf[20];
	int	revision=0;

	fd = open(MISC_TOUCH_DEV,O_RDONLY);
	if(fd < 0)
	{
		//sprintf(buf, "dev file error");
		//return (*env)->NewStringUTF(env, buf);
		return -1;
	}

	ioctl(fd,TOUCH_IOCTL_GET_CHIP_REVISION,&revision);

	close(fd);
	//revision = revision&0xffff;

	//sprintf(buf, "0x%04x", revision);
	
	//return (*env)->NewStringUTF(env, buf);
	return revision;
}


jint
Java_com_mitac_touch_TouchDebugger_GetFWversion( JNIEnv* env, jobject thiz )
{
	int fd,ret;
	char buf[20];
	int version=0;

	fd = open(MISC_TOUCH_DEV,O_RDONLY);
	if(fd < 0)
	{
		//sprintf(buf, "dev file error");
		//return (*env)->NewStringUTF(env, buf);
		return -1;
	}
	ioctl(fd,TOUCH_IOCTL_GET_FW_VERSION,&version);

	close(fd);
	//version = version&0xffff;
	//sprintf(buf, "0x%04x", version);	

	//return (*env)->NewStringUTF(env, buf);
	return version;
}

jint
Java_com_mitac_touch_TouchDebugger_GetDataVersion( JNIEnv* env, jobject thiz )
{
	int fd,ret;
	char buf[20];
	int	version=0;

	fd = open(MISC_TOUCH_DEV,O_RDONLY);
	if(fd < 0)
	{
		//sprintf(buf, "dev file error");
		//return (*env)->NewStringUTF(env, buf);
		return -1;
	}
	ioctl(fd,TOUCH_IOCTL_GET_REG_DATA_VERSION,&version);

	close(fd);

	//sprintf(buf, "0x%04x", version);

	//return (*env)->NewStringUTF(env, buf);
	return version;
}

jint
Java_com_mitac_touch_TouchDebugger_SetDebugMsg( JNIEnv* env, jobject thiz, jint value)
{
	int fd,ret;		

	fd = open(MISC_TOUCH_DEV,O_WRONLY | O_NDELAY);
	if(fd < 0)
	{
		return -1;
	}

	ret=ioctl(fd,TOUCH_IOCTL_SET_DEBUGMSG_STATE,&value);
	close(fd);
	if(ret)
	{
		return -1;
	}
	return value;
}

jint
Java_com_mitac_touch_TouchDebugger_GetDebugMsgState( JNIEnv* env, jobject thiz)
{
	int fd,ret;
	char buf[20];
	int	value;
	
	fd = open(MISC_TOUCH_DEV,O_RDONLY);
	if(fd < 0)
	{
		return -1;
	}

	ret=ioctl(fd,TOUCH_IOCTL_GET_DEBUGMSG_STATE,&value);	
	close(fd);
	if(ret)
	{
		return -1;
	}
	return value;
}

jint
Java_com_mitac_touch_TouchDebugger_GetXResolution( JNIEnv* env, jobject thiz)
{
	int fd,ret;
	char buf[20];
	int	value=0;
	
	fd = open(MISC_TOUCH_DEV,O_RDONLY);
	if(fd < 0)
	{
		return -1;
	}

	ret=ioctl(fd,TOUCH_IOCTL_GET_X_RESOLUTION,&value);	
	close(fd);
	if(ret)
	{
		return -1;
	}
	return value;
}

jint
Java_com_mitac_touch_TouchDebugger_GetYResolution( JNIEnv* env, jobject thiz)
{
	int fd,ret;
	char buf[20];
	int	value=0;
	
	fd = open(MISC_TOUCH_DEV,O_RDONLY);
	if(fd < 0)
	{
		return -1;
	}

	ret=ioctl(fd,TOUCH_IOCTL_GET_Y_RESOLUTION,&value);	
	close(fd);
	if(ret)
	{
		return -1;
	}
	return value;
}

jint
Java_com_mitac_touch_TouchDebugger_GetXNodeNum( JNIEnv* env, jobject thiz)
{
	int fd,ret;
	char buf[20];
	int	value=0;
	
	fd = open(MISC_TOUCH_DEV,O_RDONLY);
	if(fd < 0)
	{
		return -1;
	}

	ret=ioctl(fd,TOUCH_IOCTL_GET_X_NODE_NUM,&value);	
	close(fd);
	if(ret)
	{
		return -1;
	}
	return value;
}

jint
Java_com_mitac_touch_TouchDebugger_GetYNodeNum( JNIEnv* env, jobject thiz)
{
	int fd,ret;
	char buf[20];
	int	value=0;
	
	fd = open(MISC_TOUCH_DEV,O_RDONLY);
	if(fd < 0)
	{
		return -1;
	}

	ret=ioctl(fd,TOUCH_IOCTL_GET_Y_NODE_NUM,&value);	
	close(fd);
	if(ret)
	{
		return -1;
	}
	return value;
}

jint
Java_com_mitac_touch_TouchDebugger_VerifyUpgradeData( JNIEnv* env,
					jobject thiz, jbyteArray binData )
{
	int fd, ret;
	jbyte *jByte = (*env)->GetByteArrayElements(env, binData, 0);
	size_t size = (*env)->GetArrayLength(env, binData);

	//__android_log_print(ANDROID_LOG_INFO, "zinitix jni", "upgrade size = %d", size);

	fd = open(MISC_TOUCH_DEV,O_WRONLY | O_NDELAY);
	
	if(fd < 0)
	{
		return -1;
	}
	
	ret = ioctl(fd,TOUCH_IOCTL_VARIFY_UPGRADE_SIZE, &size);
	if(ret)
	{
		(*env)->ReleaseByteArrayElements(env, binData, jByte, 0);
		close(fd);
		return -2;	
	}		
	
	ret = ioctl(fd,TOUCH_IOCTL_VARIFY_UPGRADE_DATA, jByte);
		
	close(fd);

	if(ret)
	{		
		(*env)->ReleaseByteArrayElements(env, binData, jByte, 0);
		return -3;
	}
	// version
	ret = (int)((int)jByte[0]|((int)jByte[1]<<8));
	(*env)->ReleaseByteArrayElements(env, binData, jByte, 0);
	
	return ret;
}

jstring
Java_com_mitac_touch_TouchDebugger_StartUpgrade( JNIEnv* env, jobject thiz )
{
	int fd,ret;
	char buf[20];
	
	fd = open(MISC_TOUCH_DEV,O_WRONLY | O_NDELAY);
	
	if(fd < 0)
	{
		sprintf(buf, "dev file error");
		return (*env)->NewStringUTF(env, buf);
	}
	

	ret = ioctl(fd,TOUCH_IOCTL_START_UPGRADE, 0);
	close(fd);

	if(ret)
	{
		sprintf(buf, "upgrade fail");
		return (*env)->NewStringUTF(env, buf);
	}
	sprintf(buf, "upgrade success");
	return (*env)->NewStringUTF(env, buf);
}

jint
Java_com_mitac_touch_TouchDebugger_GetRegister( JNIEnv* env, jobject thiz , jint addr)
{
	int fd;		
	_reg_ioctl	reg_ioctl;
	int val;

	fd = open(MISC_TOUCH_DEV,O_RDONLY);
	if(fd < 0)
	{
		return -1;
	}
	reg_ioctl.addr = addr;
	reg_ioctl.val = (int*)&val;
	
	ioctl(fd,TOUCH_IOCTL_GET_REG,&reg_ioctl);
	close(fd);
			
	return val;
}

//===================================================================================

jint
Java_com_mitac_touch_TouchDebugger_DoHwCalibration( JNIEnv* env,
					jobject thiz )
{
	int fd,ret;	
		
	fd = open(MISC_TOUCH_DEV,O_WRONLY | O_NDELAY);
	if(fd < 0)
	{
		return -1;
	}

	ret=ioctl(fd,TOUCH_IOCTL_HW_CALIBRAION,0);
	close(fd);
	return ret;	
}

jint
Java_com_mitac_touch_TouchDebugger_NoTouchEvent( JNIEnv* env,
					jobject thiz )
{
	int fd,ret;	
		
	fd = open(MISC_TOUCH_DEV,O_WRONLY | O_NDELAY);
	if(fd < 0)
	{
		return -1;
	}

	ret=ioctl(fd,TOUCH_IOCTL_DONOT_TOUCH_EVENT,0);
	close(fd);
	return ret;	
}

jint
Java_com_mitac_touch_TouchDebugger_SetRawDataMode( JNIEnv* env,
					jobject thiz, jint mode )
{
	int fd,ret;	
	int	set_mode = TOUCH_NORMAL_MODE;
	
	fd = open(MISC_TOUCH_DEV,O_WRONLY | O_NDELAY);
	if(fd < 0)
	{
		return -1;
	}

	if(mode != 0)	set_mode = mode;

	ret=ioctl(fd,TOUCH_IOCTL_SET_RAW_DATA_MODE,&set_mode);
	close(fd);
	if(ret)
	{
		return -2;
	}
	return 0;
}


jint
Java_com_mitac_touch_TouchDebugger_GetRawData( JNIEnv* env,
					jobject thiz, jintArray rawData , jint sz)
{
	
	int fd,ret;
	_raw_ioctl raw_ioctl;
	jbyte *jByte = (*env)->GetByteArrayElements(env, rawData, 0);
	char *buf;
	int i;
	
	fd = open(MISC_TOUCH_DEV,O_RDONLY);
	if(fd < 0)
	{		
		return -1;
	}	
	buf = (char*)malloc(sz);
	if(buf == 0)	return -1;
	raw_ioctl.sz = sz;
	raw_ioctl.buf = buf;
	ret=ioctl(fd,TOUCH_IOCTL_GET_RAW_DATA, &raw_ioctl);	
	close(fd);
	
	if(ret == 0)
	{
		for(i=0; i<sz; i++)
			jByte[i] = (jbyte)buf[i];
	}

	(*env)->ReleaseByteArrayElements(env, rawData, jByte, 0);
	
	free(buf);
	if(ret == -1)	
		return ret;	
	if(ret == -2)	// no update
		return 1;
	return 0;
}

jint
Java_com_mitac_touch_TouchDebugger_SetRegister( JNIEnv* env, jobject thiz , jint addr, jint val)
{
	int fd,ret;		
	_reg_ioctl	reg_ioctl;

	fd = open(MISC_TOUCH_DEV,O_WRONLY | O_NDELAY);
	if(fd < 0)
	{
		return -1;
	}
	reg_ioctl.addr = addr;
	reg_ioctl.val = (int*)&val;
		
	ret = ioctl(fd,TOUCH_IOCTL_SET_REG,&reg_ioctl);
	close(fd);
	
	return ret;
}
//====================================================================================


jint
Java_com_mitac_touch_TouchDebugger_SendSaveStatus( JNIEnv* env, jobject thiz)
{
	int fd,ret;		

	fd = open(MISC_TOUCH_DEV,O_WRONLY | O_NDELAY);
	if(fd < 0)
	{
		return -1;
	}	
	
	ret = ioctl(fd,TOUCH_IOCTL_SEND_SAVE_STATUS,0);
	close(fd);	
	return ret;
}

jint
Java_com_mitac_touch_TouchDebugger_SetRegisterSeq( JNIEnv* env, jobject thiz , jint addr, jint val)
{
	int fd,ret;		
	_reg_ioctl	reg_ioctl;

	fd = open(MISC_TOUCH_DEV,O_WRONLY | O_NDELAY);
	if(fd < 0)
	{
		return -1;
	}
	reg_ioctl.addr = addr;
	reg_ioctl.val = (int*)&val;
		
	ret = ioctl(fd,TOUCH_IOCTL_SET_REG,&reg_ioctl);
	if(ret)
	{
		close(fd);
		return ret;
	}
		
	ret = ioctl(fd,TOUCH_IOCTL_SEND_SAVE_STATUS,0);
	if(ret)
	{
		close(fd);
		return ret;
	}
		
	ret=ioctl(fd,TOUCH_IOCTL_HW_CALIBRAION,0);
	if(ret)
	{
		close(fd);
		return ret;
	}
	
	close(fd);
	
	return ret;
}

//==============================================================================================================

