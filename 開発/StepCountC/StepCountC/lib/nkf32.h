// for nkf32.dll (nkf15k����)
// �g�p���@�A�]�ځA�g�p�����́A������ nkf32.doc���Q�Ƃ��ĉ������B

// �����ϊ� (NKF)
void CALLBACK GetNkfVersion(LPSTR) ;
int  CALLBACK SetNkfOption(LPCSTR) ;
void CALLBACK NkfConvert(LPSTR,LPCSTR) ;

// ���p�E�S�p�ϊ�
void CALLBACK ToHankaku(LPSTR) ;			// (V1.02)
void CALLBACK ToZenkakuKana(LPSTR,LPCSTR) ;	// (V1.02)

// Subject�ϊ�(to MIME)
void CALLBACK EncodeSubject(LPSTR,LPCSTR) ;	// (V1.03)
