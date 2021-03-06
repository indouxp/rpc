/*
 * コンピューター：C言語講座：RPCについて(1)
 * http://www.ncad.co.jp/~komata/c-kouza15.htm
 *
 */
#include <sys/types.h>
#include <dirent.h>
#include <rpc/rpc.h>

#define BUF_SIZE 8192
#define RLS_PROG ((u_long)0x20000000)
#define RLS_VER ((u_long)1)
#define RLS_PROC ((u_long)1)

void main()
{
bool_t   xdr_rls();
char     *do_rls();

  // サービス登録
  // 1:プログラム番号
  // 2:バージョン番号
  // 3:関数番号
  // 4:do_rls  リクエスト時に呼ばれる関数
  // 5:xdr_rls アーキテクチャーの異なるマシン間でもデータをやり取りするためにxdrライブラリを使用
  registerrpc(RLS_PROG, RLS_VER, RLS_PROC, do_rls, xdr_rls, xdr_rls);
  // リクエスト待ち 
  svc_run();
}

char *do_rls(buf)
char *buf;
{
DIR     *dirp;
struct dirent   *dp;

  dirp=opendir(buf);
  if(dirp==NULL){
    strcpy(buf,"opendir error");
    return(NULL);
  }
  buf[0]=NULL;
  while(dp=readdir(dirp)){
    sprintf(buf,"%s%s\n",buf,dp->d_name);
  }
  closedir(dirp);
  return(buf);
}

bool_t xdr_rls(xdrs,objp)
XDR *xdrs;
char *objp;
{
  return(xdr_string(xdrs,&objp,BUF_SIZE));
}
