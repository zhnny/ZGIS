/************************************************************************/
/*                             DBF Support.                             */
/************************************************************************/
#ifndef _DBFOPEN_H
#define _DBFOPEN_H

#include <stdio.h>

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

typedef	struct
{
    FILE	*fp;

    int         nRecords;

    int		nRecordLength;
    int		nHeaderLength;
    int		nFields;
    int		*panFieldOffset;
    int		*panFieldSize;
    int		*panFieldDecimals;
    char	*pachFieldType;

    char	*pszHeader;

    int		nCurrentRecord;
    int		bCurrentRecordModified;
    char	*pszCurrentRecord;
    
    int		bNoHeader;
    int		bUpdated;
} DBFInfo;

typedef DBFInfo * DBFHandle;

typedef enum {
  FTString,
  FTInteger,
  FTDouble,
  FTInvalid
} DBFFieldType;

#define XBASE_FLDHDR_SZ       32

DBFHandle DBFOpen( const char * pszDBFFile, const char * pszAccess );
DBFHandle DBFCreate( const char * pszDBFFile );

int	DBFGetFieldCount( DBFHandle psDBF );
int	DBFGetRecordCount( DBFHandle psDBF );
int	DBFAddField( DBFHandle hDBF, const char * pszFieldName,
		     DBFFieldType eType, int nWidth, int nDecimals );

DBFFieldType DBFGetFieldInfo( DBFHandle psDBF, int iField, 
			      char * pszFieldName, 
			      int * pnWidth, int * pnDecimals );

int 	DBFReadIntegerAttribute( DBFHandle hDBF, int iShape, int iField );
double 	DBFReadDoubleAttribute( DBFHandle hDBF, int iShape, int iField );
const char *DBFReadStringAttribute( DBFHandle hDBF, int iShape, int iField );

int DBFWriteIntegerAttribute( DBFHandle hDBF, int iShape, int iField, 
			      int nFieldValue );
int DBFWriteDoubleAttribute( DBFHandle hDBF, int iShape, int iField,
			     double dFieldValue );
int DBFWriteStringAttribute( DBFHandle hDBF, int iShape, int iField,
			     const char * pszFieldValue );

const char *DBFReadTuple(DBFHandle psDBF, int hEntity );
int DBFWriteTuple(DBFHandle psDBF, int hEntity, void * pRawTuple );

DBFHandle DBFCloneEmpty(DBFHandle psDBF, const char * pszFilename );
 
void	DBFClose( DBFHandle hDBF );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _DBFOPEN_H