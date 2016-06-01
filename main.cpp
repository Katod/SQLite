
#define	Version	"000"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<syslog.h>
#include	<sqlite3.h>


static void databaseError(sqlite3* db){
        int errcode = sqlite3_errcode(db);
        const char *errmsg = sqlite3_errmsg(db);
        fprintf(stderr, "Database error %d: %sn", errcode, errmsg);
}


static int readBlob(
                sqlite3 *db, /* Database containing blobs table */
                const char *zKey, /* Null-terminated key to retrieve blob for */
                unsigned char **pzBlob, /* Set *pzBlob to point to the retrieved blob */
                int *pnBlob /* Set *pnBlob to the size of the retrieved blob */
                ){
        const char *zSql = "SELECT id FROM clients WHERE phone_number = 123";
        sqlite3_stmt *pStmt;
        int rc;//TEST aaa

        /* In case there is no table entry for key zKey or an error occurs,
         ** set *pzBlob and *pnBlob to 0 now.
         */
        *pzBlob = 0;
        *pnBlob = 0;

        do {
                /* Compile the SELECT statement into a virtual machine. */
                rc = sqlite3_prepare(db, zSql, -1, &pStmt, 0);
                 if( rc!=SQLITE_OK ){
                        return rc;
                }

                /* Bind the key to the SQL variable. */
                sqlite3_bind_text(pStmt, 1, zKey, -1, SQLITE_STATIC);

                /* Run the virtual machine. We can tell by the SQL statement that
                 ** at most 1 row will be returned. So call sqlite3_step() once
                 ** only. Normally, we would keep calling sqlite3_step until it
                 ** returned something other than SQLITE_ROW.
                 */
                rc = sqlite3_step(pStmt);
                if( rc==SQLITE_ROW ){
                        /* The pointer returned by sqlite3_column_blob() points to memory
                         ** that is owned by the statement handle (pStmt). It is only good
                         ** until the next call to an sqlite3_XXX() function (e.g. the
                         ** sqlite3_finalize() below) that involves the statement handle.
                         ** So we need to make a copy of the blob into memory obtained from
                         ** malloc() to return to the caller.
                         */
                        *pnBlob = sqlite3_column_bytes(pStmt, 0);
                        *pzBlob = (unsigned char *)malloc(*pnBlob);
                        memcpy(*pzBlob, (void *)sqlite3_column_blob(pStmt, 0), *pnBlob);
                }

                /* Finalize the statement (this releases resources allocated by
                 ** sqlite3_prepare() ).
                 */
                rc = sqlite3_finalize(pStmt);

                /* If sqlite3_finalize() returned SQLITE_SCHEMA, then try to execute
                 ** the statement all over again.
                 */
        } while( rc==SQLITE_SCHEMA );

        return rc;
}

int main(
  int argc,
  char **argv )
{
  
  sqlite3* pDB;
  int nBlob;
  unsigned char *zBlob;
  char const *zFile;
  
  if(sqlite3_open("/home/katod/projects/Hello/build/GSM",&pDB) == SQLITE_OK)
    printf("Opened db successfully\n");
  else
    printf("Failed to open db\n");
  
  
if( SQLITE_OK!=readBlob(pDB, zFile, &zBlob, &nBlob) )
{
  databaseError(pDB);
  return 1;
}

if( !zBlob )
{
  fprintf(stderr, "No such database entry: %sn", zFile);
  return 1;
}
  
//  auto	char	event[256];
//  auto	int	idx;
// 
//   // Set the syslog ident
//   openlog("mfe", LOG_PID, LOG_USER);
// 
//   // announce ourselves
//   syslog(LOG_ERR, "Starting.");
// 
//   // show how we were executed
//   idx = 0;
//   syslog(LOG_ERR, "argc = %d", argc);
// 
//   while	(idx < argc)
//   {
//     syslog(LOG_ERR, "arg[%d] = \"%s\"", idx, argv[idx]);
//     ++idx;
//   }    
// // read events
// while(NULL != fgets(event, sizeof(event), stdin))
// {
//   syslog(LOG_ERR, event);
//   if('1' == *event)
//   {
//     printf("S,demo-congrats\n");
//     fflush(stdout);
//   }
//   if('2' == *event)
//   {
//     printf("S,/home/katod/Downloads/example\n");
//     fflush(stdout);
//   }
//   
//   if('3' == *event)
//   {
//     printf("S,/home/katod/Downloads/test\n");
//     fflush(stdout);
//   }
// 
// if('#' == *event)
// {
//   break;
// }
// }

// Function exit
return(EXIT_SUCCESS);// return function status
}// end of main()