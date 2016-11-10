

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/

#ifndef __HISI_LOAD_IMAGE_H__
#define __HISI_LOAD_IMAGE_H__


/*****************************************************************************
  1 STRUCT����
*****************************************************************************/

/***************load_image struct for new platform from chicago**************/
struct load_image_info {
	unsigned int ecoretype;
	unsigned long image_addr;
	unsigned int image_size;
	char *partion_name;
};

/*****************************************************************************
  3 ��������
*****************************************************************************/

/*new func for new platform from chicago*/
int bsp_load_and_verify_image(struct load_image_info *img_info);

#endif /* end of hisi_load_image.h */
