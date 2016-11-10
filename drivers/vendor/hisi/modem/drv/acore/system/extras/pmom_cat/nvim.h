

enum
{
    NV_ICC_REQ_PRIORITY_HIGH_FLUSH      = 0x40,         /*ccore����д������,��Ҫ�ȴ��ظ�*/
    NV_ICC_REQ_PRIORITY1_FLUSH          = 0x41,         /*ccore �ӳ�д�����󣬲���Ҫ�ȴ��ظ�*/
    NV_ICC_REQ_PRIORITY_MID_LOW_FLUSH   = 0x42,
    NV_ICC_REQ_SHUT_DOWN_FLUSH          = 0x43,         /*ccore �ػ�д������Ϣ*/
    NV_ICC_REQ_RESUME                   = 0x44,         /*����ָ�DDR�е�nv����*/
    NV_ICC_REQ_RESUME_ITEM              = 0x45,         /*����ָ�DDR�е�nv��������*/
    NV_ICC_CNF                          = 127,
    NV_ICC_BUTT                         = 0xFF,
};
#define NV_ICC_MSG_TYPE_SUM (7)
#define NV_ICC_MSG_ACORE    0
#define NV_ICC_MSG_CCORE    1


/*ICC����NV_ICC_REQ_RESUME NV_ICC_REQ_RESUME��Ϣ�ṹ*/
typedef struct _nv_icc_msg_stru_
{
    u32 send_slice;         /*����ʱ��*/
    u32 msg_type;           /*��Ϣ����*/
    u32 sn;                 /*���к�*/
    u32 ret;                /*������Ϣ�д洢Ҫд���NV����,������Ϣ�д洢������*/
}nv_icc_msg_t;

/*��������Nv��Ϣ*/
typedef struct _nv_item_info_stru_
{
    u16 itemid;      /* NV ID */
    u16 modem_id;      /* NV Length */
}nv_item_info_t;

/*ICC����NV_ICC_REQ_PRIORITY_HIGH_FLUSH NV_ICC_REQ_PRIORITY1_FLUSH��Ϣ�ṹ*/
typedef struct _nv_icc_msg_single_stru_
{
    u32 send_slice;         /*����ʱ��*/
    u32 msg_type;           /*��Ϣ����*/
    u32 sn;                 /*���к�*/
    u32 ret;                /*������Ϣ�д洢Ҫд���NV����,������Ϣ�д洢������*/
    nv_item_info_t data;    /*Ҫд���nv��Ϣ�б�*/
}nv_icc_msg_single_t;

struct nv_icc_msg_name
{
    unsigned int msg_type;
    char         cpu[5];
    char         work[32];
};

int nvim_msg_parse(unsigned int typeid, char *in_buf, unsigned int len, char *out_buf, unsigned int *out_ptr);


