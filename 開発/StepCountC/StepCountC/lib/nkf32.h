// for nkf32.dll (nkf15k相当)
// 使用方法、転載、使用条件は、同梱の nkf32.docを参照して下さい。

// 漢字変換 (NKF)
void CALLBACK GetNkfVersion(LPSTR) ;
int  CALLBACK SetNkfOption(LPCSTR) ;
void CALLBACK NkfConvert(LPSTR,LPCSTR) ;

// 半角・全角変換
void CALLBACK ToHankaku(LPSTR) ;			// (V1.02)
void CALLBACK ToZenkakuKana(LPSTR,LPCSTR) ;	// (V1.02)

// Subject変換(to MIME)
void CALLBACK EncodeSubject(LPSTR,LPCSTR) ;	// (V1.03)
