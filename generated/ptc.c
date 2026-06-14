/*
**       Code derived from program ptc
*/
# include <stdlib.h>
/*
**     Definitions for i/o
*/
# include <stdio.h>
typedef struct {
 FILE *fp;
 unsigned short eoln:1,
                     eof:1,
                     out:1,
                     init:1,
                     :12;
 char buf;
}  text;
text input = { 0, 0, 0 };
text output = { 0, 0, 0 };
# define Fread(x, f) fread((char *)&x, sizeof(x), 1, f)
# define Get(f) Fread((f).buf, (f).fp)
# define Getx(f) (f).init = 1, (f).eoln = (((f).buf = fgetc((f).fp)) == '\n') ? (((f).buf = ' '), 1) : 0
# define Getchr(f) (f).buf, Getx(f)
static FILE *Tmpfil;
static long Tmplng;
static double Tmpdbl;
# define Fscan(f) (f).init ? ungetc((f).buf, (f).fp) : 0, Tmpfil = (f).fp
# define Scan(p, a) Scanck(fscanf(Tmpfil, p, a))
void Scanck();
# define Eoln(f) ((f).eoln ? true : false)
# define Eof(f) ((((f).init == 0) ? (Get(f)) : 0, ((f).eof ? 1 : feof((f).fp))) ? true : false)
# define Fwrite(x, f) fwrite((char *)&x, sizeof(x), 1, f)
# define Put(f) Fwrite((f).buf, (f).fp)
# define Putx(f) (f).eoln = ((f).buf == '\n'), (void)fputc((f).buf, (f).fp)
# define Putchr(c, f) (f).buf = (c), Putx(f)
# define Putl(f, v) (f).eoln = v
/*
**     Definitions for case-statements
**     and for non-local gotos
*/
# define Line __LINE__
static void Caseerror();
# include <setjmp.h>
static struct Jb { jmp_buf jb; } J[1];
/*
**     Definitions for standard types
*/
extern int strncmp();
# define Cmpstr(x, y) strncmp((x), (y), sizeof(x))
typedef char boolean;
# define false (boolean)0
# define true (boolean)1
static char *Bools[];
typedef int integer;
# define maxint 2147483647
/*
**     Definitions for pointers
*/
# ifndef Unionoffs
# define Unionoffs(p, m) (((long)(&(p)->m))-((long)(p)))
# endif
# define NIL 0
/*
**     Definitions for set-operations
*/
# define Claimset() (void)Currset(0, (setptr)0)
# define Newset() Currset(1, (setptr)0)
# define Saveset(s) Currset(2, s)
# define setbits 15
typedef unsigned short setword;
typedef setword * setptr;
static boolean Member(), Le(), Ge(), Eq(), Ne();
static setptr Union(), Diff();
static setptr Insmem(), Mksubr();
static setptr Currset(), Inter();
static setptr Tmpset;
static setptr Conset[];
static void Setncpy();
# ifndef SETALIGN
# define SETALIGN(x) Alignset(x)
static struct Set { unsigned short S[15+1]; } *Alignset();
# endif
# ifndef STRALIGN
# define STRALIGN(x) Alignstr(x)
static struct String { char   A[127+1]; } *Alignstr();
# endif
extern char *strncpy();
/*
**     Start of program definitions
*/
static char version[]       = "@(#)ptc.p    2.6  Date 87/09/12";
# define keytablen 38
# define keywordlen 10
static char othersym[]       = "otherwise ";
static char externsym[]       = "external  ";
static char dummysym[]       = "          ";
static char wordtype[]       = "unsigned short";
# define C37_setbits 15
static char filebits[]       = "unsigned short";
# define filefill 12
# define maxsetrange 15
# define scalbase 0
# define maxprio 7
# define maxmachdefs 8
# define machdeflen 16
# define maxstrblk 1023
# define maxblkcnt 63
# define maxstrstor 65535
# define maxtoknlen 127
# define hashmax 64
# define null 0
# define minchar null
# define maxchar 127
static char tmpfilename[]       = "\"/tmp/ptc%d%c\", getpid(), ";
static char maxfilename[]       = "MAXFILENAME";
# define space ' '
# define tab1 ' '
static char tab2[]       = "             ";
static char tab3[]       = "                     ";
static char tab4[]       = "                             ";
# define bslash '\\'
static char nlchr[]       = "'\\n'";
static char ffchr[]       = "'\\f'";
static char nulchr[]       = "'\\0'";
static char spchr[]       = "' '";
# define quote '\''
# define cite '"'
# define xpnent 'e'
# define percent '%'
# define uscore '_'
# define badchr '?'
# define okchr quote
# define tabwidth 8
# define echo false
# define diffcomm false
# define lazyfor false
# define unionnew true
static char inttyp[]       = "int";
static char chartyp[]       = "char";
static char setwtyp[]       = "setword";
static char setptyp[]       = "setptr";
static char floattyp[]       = "float";
static char doubletyp[]       = "double";
static char dblcast[]       = "(double)";
# define realtyp doubletyp
static char voidtyp[]       = "void";
static char voidcast[]       = "(void)";
# define align true
# define intlen 10
# define fixlen 20
static char C24_include[]       = "# include ";
static char C4_define[]       = "# define ";
static char C53_undef[]       = "# undef ";
static char ifdef[]       = "# ifdef ";
static char ifndef[]       = "# ifndef ";
static char elsif[]       = "# else";
static char endif[]       = "# endif";
static char C50_static[]       = "static ";
static char xtern[]       = "extern ";
static char typdef[]       = "typedef ";
static char registr[]       = "register ";
# define indstep 8
typedef unsigned char hashtyp;
typedef unsigned short strindx;
typedef unsigned short strbidx;
typedef struct { char A[maxstrblk + 1]; } strblk;
typedef strblk * strptr;
typedef unsigned char strbcnt;
typedef struct S62 * idptr;
typedef struct S62 {
 idptr inext;
 unsigned char inref;
 hashtyp ihash;
 strindx istr;
} idnode;
typedef unsigned char toknidx;
typedef struct { char A[maxtoknlen - 1 + 1]; } toknbuf;
typedef struct { char A[keywordlen - 1 + 1]; } keyword;
typedef enum { dabs, darctan, dargc, dargv,
 dboolean, dchar, dchr, dclose,
 dcos, ddispose, deof, deoln,
 dexit, dexp, dfalse, dflush,
 dget, dhalt, dinput, dinteger,
 dln, dmaxint, dmessage, dnew,
 dodd, dord, doutput, dpage,
 dpack, dpred, dput, dread,
 dreadln, dreal, dreset, drewrite,
 dround, dsin, dsqr, dsqrt,
 dsucc, dtext, dtrue, dtrunc,
 dtan, dwrite, dwriteln, dunpack,
 dzinit, dztring }  predefs;
typedef enum { sand, sarray, sbegin, scase,
 sconst, sdiv, sdo, sdownto,
 selse, send, sextern, sfile,
 sfor, sforward, sfunc, sgoto,
 sif, sinn, slabel, smod,
 snil, snot, sof, sor,
 sother, spacked, sproc, spgm,
 srecord, srepeat, sset, sthen,
 sto, stype, suntil, svar,
 swhile, swith, seof, sinteger,
 sreal, sstring, schar, sid,
 splus, sminus, smul, squot,
 sarrow, slpar, srpar, slbrack,
 srbrack, seq, sne, slt,
 sle, sgt, sge, scomma,
 scolon, ssemic, sassign, sdotdot,
 sdot }  symtyp;
typedef struct { setword S[6]; } symset;
typedef struct S206 {
 symtyp st;
 union {
  struct  {
   idptr vid;
  } V1;
  struct  {
   char vchr;
  } V2;
  struct  {
   integer vint;
  } V3;
  struct  {
   strindx vflt;
  } V4;
  struct  {
   strindx vstr;
  } V5;
 } U;
} lexsym;
typedef enum { lpredef, lidentifier, lfield, lforward,
 lpointer, lstring, llabel, lforwlab,
 linteger, lreal, lcharacter }  ltypes;
typedef struct S63 * declptr;
typedef struct S64 * treeptr;
typedef struct S65 * symptr;
typedef struct S65 {
 treeptr lsymdecl;
 symptr lnext;
 declptr ldecl;
 ltypes lt;
 union {
  struct  {
   idptr lid;
   boolean lused;
  } V6;
  struct  {
   strindx lstr;
  } V7;
  struct  {
   strindx lfloat;
  } V8;
  struct  {
   integer lno;
   boolean lgo;
  } V9;
  struct  {
   integer linum;
  } V10;
  struct  {
   char lchar;
  } V11;
 } U;
} symnode;
typedef struct S63 {
 declptr dprev;
 struct { symptr A[hashmax + 1]; } ddecl;
} declnode;
typedef enum { npredef, npgm, nfunc, nproc,
 nlabel, nconst, ntype, nvar,
 nvalpar, nvarpar, nparproc, nparfunc,
 nsubrange, nvariant, nfield, nrecord,
 narray, nconfarr, nfileof, nsetof,
 nbegin, nptr, nscalar, nif,
 nwhile, nrepeat, nfor, ncase,
 nchoise, ngoto, nwith, nwithvar,
 nempty, nlabstmt, nassign, nformat,
 nin, neq, nne, nlt,
 nle, ngt, nge, nor,
 nplus, nminus, nand, nmul,
 ndiv, nmod, nquot, nnot,
 numinus, nuplus, nset, nrange,
 nindex, nselect, nderef, ncall,
 nid, nchar, ninteger, nreal,
 nstring, nnil, npush, npop,
 nbreak }  treetyp;
typedef enum { tnone, tboolean, tchar, tinteger,
 treal, tstring, tnil, tset,
 ttext, tpoly, terror }  pretyps;
typedef enum { anone, aregister, aextern, areference }  attributes;
typedef struct S64 {
 treeptr tnext, ttype, tup;
 integer tline, tcol;
 boolean tsemchecked, tsemchecking;
 treetyp tt;
 union {
  struct  {
   predefs tdef;
   pretyps tobtyp;
  } V12;
  struct  {
   treeptr tsubid, tsubpar, tfuntyp, tsublab,
    tsubconst, tsubtype, tsubvar, tsubsub,
    tsubstmt;
   integer tstat;
   declptr tscope;
  } V13;
  struct  {
   treeptr tidl, tbind;
   attributes tattr;
  } V14;
  struct  {
   treeptr tparid, tparparm, tpartyp;
  } V15;
  struct  {
   treeptr tptrid;
   boolean tptrflag;
  } V16;
  struct  {
   treeptr tscalid;
  } V17;
  struct  {
   treeptr tof;
  } V18;
  struct  {
   treeptr tlo, thi;
  } V19;
  struct  {
   treeptr tselct, tvrnt;
  } V20;
  struct  {
   treeptr tflist, tvlist;
   idptr tuid;
   declptr trscope;
  } V21;
  struct  {
   treeptr tcindx, tindtyp, tcelem;
   idptr tcuid;
  } V22;
  struct  {
   treeptr taindx, taelem;
  } V23;
  struct  {
   treeptr tbegin;
  } V24;
  struct  {
   treeptr tlabno, tstmt;
  } V25;
  struct  {
   treeptr tlabel;
  } V26;
  struct  {
   treeptr tlhs, trhs;
  } V27;
  struct  {
   treeptr tglob, tloc, ttmp;
  } V28;
  struct  {
   treeptr tbrkid, tbrkxp;
  } V29;
  struct  {
   treeptr tcall, taparm;
  } V30;
  struct  {
   treeptr tifxp, tthen, telse;
  } V31;
  struct  {
   treeptr twhixp, twhistmt;
  } V32;
  struct  {
   treeptr treptstmt, treptxp;
  } V33;
  struct  {
   treeptr tforid, tfrom, tto, tforstmt;
   boolean tincr;
  } V34;
  struct  {
   treeptr tcasxp, tcaslst, tcasother;
  } V35;
  struct  {
   treeptr tchocon, tchostmt;
  } V36;
  struct  {
   treeptr twithvar, twithstmt;
  } V37;
  struct  {
   treeptr texpw;
   declptr tenv;
  } V38;
  struct  {
   treeptr tvariable, toffset;
  } V39;
  struct  {
   treeptr trecord, tfield;
  } V40;
  struct  {
   treeptr texpl, texpr;
  } V41;
  struct  {
   treeptr texps;
   boolean tuplusop;
  } V42;
  struct  {
   symptr tsym;
  } V43;
 } U;
} treenode;
typedef enum { cabort, cbreak, ccontinue, cdefine,
 cdefault, cdouble, cedata, cenum,
 cetext, cextern, cfgetc, cfclose,
 cfflush, cfloat, cfloor, cfprintf,
 cfputc, cfread, cfscanf, cfwrite,
 cgetc, cgetpid, cint, cinclude,
 clong, clog, cmain, cmalloc,
 cprintf, cpower, cputc, cread,
 creturn, cregister, crewind, cscanf,
 csetbits, csetword, csetptr, cshort,
 csigned, csizeof, csprintf, cstdin,
 cstdout, cstderr, cstrncmp, cstrncpy,
 cstruct, cstatic, cswitch, ctypedef,
 cundef, cungetc, cunion, cunlink,
 cfseek, cgetchar, cputchar, cunsigned,
 cwrite }  cnames;
typedef enum { ebadsymbol, elongstring, elongtokn, erange,
 emanytokn, enotdeclid, emultdeclid, enotdecllab,
 emultdecllab, emuldeflab, ebadstring, enulchr,
 ebadchar, eeofcmnt, eeofstr, evarpar,
 enew, esetbase, esetsize, eoverflow,
 etree, etag, euprconf, easgnconf,
 ecmpconf, econfconf, evrntfile, evarfile,
 emanymachs, ebadmach, eprconf, easgntype,
 eoperand, eargcount, eargtype, efunrtype,
 eindex, enotrecord, enotpointer, eforctrl,
 eboolxp, ecasetype, edupcase, enofunresult,
 eprocexpr, eidkind, etypeexpected, econstexpected,
 ecalltarget, efuncstmt, ebadrange }  errors;
typedef struct { char A[machdeflen - 1 + 1]; } machdefstr;
typedef struct { struct S232 {
 keyword wrd;
 symtyp sym;
} A[keytablen + 1]; } T66;
typedef struct { strptr A[maxblkcnt + 1]; } T67;
typedef struct { idptr A[hashmax + 1]; } T68;
typedef struct { treeptr A[50]; } T69;
typedef struct { symptr A[50]; } T70;
typedef struct { treeptr A[11]; } T71;
typedef struct { unsigned char A[(int)(nnil) - (int)(nassign) + 1]; } T72;
typedef struct { idptr A[61]; } T73;
typedef struct { struct S219 {
 integer lolim, hilim;
 strindx typstr;
} A[maxmachdefs - 1 + 1]; } T74;
typedef struct { char A[15 + 1]; } T75;
typedef struct { setword S[2]; } bitset;
integer *G230_indnt;
boolean *G228_donearr;
boolean *G226_doarrow;
boolean *G224_dropset;
boolean *G222_setused;
boolean *G220_conflag;
integer *G217_nelems;
treeptr *G215_vp;
treeptr *G213_tv;
symptr *G211_iq;
symptr *G209_ip;
unsigned char *G207_lastchr;
toknidx *G204_i;
toknbuf *G202_t;
boolean usemax, usejmps, usecase, usesets, useunion, usediff,
 usemksub, useintr, usesge, usesle, useseq, usesne,
 usememb, useins, usescpy, usecomp, usealig, usesal,
 usefopn, usescan, usegetl, usenilp, usebool;
treeptr top;
treeptr setlst;
integer setcnt;
lexsym currsym;
T66 keytab;
T67 strstor;
strindx strfree;
strbidx strleft;
T68 idtab;
declptr symtab;
integer statlvl, maxlevel;
T69 deftab;
T70 defnams;
T71 typnods;
T72 pprio, cprio;
T73 ctable;
unsigned char nmachdefs;
T74 machdefs;
integer lineno, colno, lastcol, lastline;
toknbuf lasttok;
integer varno;
char pushchr;
boolean pushed;
T75 hexdig;

 void
prtmsg(m)
 errors m;
{
 static char user[]       = "Error: ";
 static char restr[]       = "Implementation restriction: ";
 static char inter[]       = "* Internal error * ";
# define xtoklen 64
 typedef struct { char A[xtoklen - 1 + 1]; } T76;
 toknidx i;
 T76 xtok;

 switch ((int)(m)) {
   case ebadsymbol:
  (void)fprintf(stderr, "%sUnexpected symbol\n", user), Putl(output, 1);
  break ;
   case ebadchar:
  (void)fprintf(stderr, "%sBad character\n", user), Putl(output, 1);
  break ;
   case elongstring:
  (void)fprintf(stderr, "%sToo long string\n", restr), Putl(output, 1);
  break ;
   case ebadstring:
  (void)fprintf(stderr, "%sNewline in string or character\n", user), Putl(output, 1);
  break ;
   case eeofstr:
  (void)fprintf(stderr, "%sEnd of file in string or character\n", user), Putl(output, 1);
  break ;
   case eeofcmnt:
  (void)fprintf(stderr, "%sEnd of file in comment\n", user), Putl(output, 1);
  break ;
   case elongtokn:
  (void)fprintf(stderr, "%sToo long identfier\n", restr), Putl(output, 1);
  break ;
   case emanytokn:
  (void)fprintf(stderr, "%sToo many strings, identifiers or real numbers\n", restr), Putl(output, 1);
  break ;
   case enotdeclid:
  (void)fprintf(stderr, "%sIdentifier not declared\n", user), Putl(output, 1);
  break ;
   case emultdeclid:
  (void)fprintf(stderr, "%sIdentifier declared twice\n", user), Putl(output, 1);
  break ;
   case enotdecllab:
  (void)fprintf(stderr, "%sLabel not declared\n", user), Putl(output, 1);
  break ;
   case emultdecllab:
  (void)fprintf(stderr, "%sLabel declared twice\n", user), Putl(output, 1);
  break ;
   case emuldeflab:
  (void)fprintf(stderr, "%sLabel defined twice\n", user), Putl(output, 1);
  break ;
   case evarpar:
  (void)fprintf(stderr, "%sActual parameter not a variable\n", user), Putl(output, 1);
  break ;
   case enulchr:
  (void)fprintf(stderr, "%sCannot handle nul-character in strings\n", restr), Putl(output, 1);
  break ;
   case enew:
  (void)fprintf(stderr, "%sNew returned a nil-pointer\n", restr), Putl(output, 1);
  break ;
   case eoverflow:
  (void)fprintf(stderr, "%sToken buffer overflowed\n", restr), Putl(output, 1);
  break ;
   case esetbase:
  (void)fprintf(stderr, "%sCannot handle sets with base >> 0\n", restr), Putl(output, 1);
  break ;
   case esetsize:
  (void)fprintf(stderr, "%sCannot handle sets with very large range\n", restr), Putl(output, 1);
  break ;
   case etree:
  (void)fprintf(stderr, "%sBad tree structure\n", inter), Putl(output, 1);
  break ;
   case etag:
  (void)fprintf(stderr, "%sCannot find tag\n", inter), Putl(output, 1);
  break ;
   case evrntfile:
  (void)fprintf(stderr, "%sCannot initialize files in record variants\n", restr), Putl(output, 1);
  break ;
   case evarfile:
  (void)fprintf(stderr, "%sCannot handle files in structured variables\n", restr), Putl(output, 1);
  break ;
   case euprconf:
  (void)fprintf(stderr, "%sNo upper bound on conformant arrays\n", inter), Putl(output, 1);
  break ;
   case easgnconf:
  (void)fprintf(stderr, "%sCannot assign conformant arrays\n", inter), Putl(output, 1);
  break ;
   case ecmpconf:
  (void)fprintf(stderr, "%sCannot compare conformant arrays\n", inter), Putl(output, 1);
  break ;
   case econfconf:
  (void)fprintf(stderr, "%sCannot handle nested conformat arrays\n", restr), Putl(output, 1);
  break ;
   case erange:
  (void)fprintf(stderr, "%sCannot find C-type for integer-subrange\n", inter), Putl(output, 1);
  break ;
   case emanymachs:
  (void)fprintf(stderr, "%sToo many machine integer types\n", restr), Putl(output, 1);
  break ;
   case ebadmach:
  (void)fprintf(stderr, "%sBad name for machine integer type\n", inter), Putl(output, 1);
  break ;
   case eprconf:
  (void)fprintf(stderr, "%sCannot write conformant arrays\n", restr), Putl(output, 1);
  break ;
   case easgntype:
  (void)fprintf(stderr, "%sIncompatible types in assignment\n", user), Putl(output, 1);
  break ;
   case eoperand:
  (void)fprintf(stderr, "%sInvalid operand type for operator\n", user), Putl(output, 1);
  break ;
   case eargcount:
  (void)fprintf(stderr, "%sWrong number of arguments\n", user), Putl(output, 1);
  break ;
   case eargtype:
  (void)fprintf(stderr, "%sIncompatible argument type\n", user), Putl(output, 1);
  break ;
   case efunrtype:
  (void)fprintf(stderr, "%sInvalid function result type\n", user), Putl(output, 1);
  break ;
   case eindex:
  (void)fprintf(stderr, "%sInvalid array index or indexed object\n", user), Putl(output, 1);
  break ;
   case enotrecord:
  (void)fprintf(stderr, "%sField selection requires a record\n", user), Putl(output, 1);
  break ;
   case enotpointer:
  (void)fprintf(stderr, "%sDereference requires a pointer or file\n", user), Putl(output, 1);
  break ;
   case eforctrl:
  (void)fprintf(stderr, "%sInvalid for control variable or bounds\n", user), Putl(output, 1);
  break ;
   case eboolxp:
  (void)fprintf(stderr, "%sBoolean expression required\n", user), Putl(output, 1);
  break ;
   case ecasetype:
  (void)fprintf(stderr, "%sInvalid case selector or label type\n", user), Putl(output, 1);
  break ;
   case edupcase:
  (void)fprintf(stderr, "%sDuplicate case label\n", user), Putl(output, 1);
  break ;
   case enofunresult:
  (void)fprintf(stderr, "%sFunction result is never assigned\n", user), Putl(output, 1);
  break ;
   case eprocexpr:
  (void)fprintf(stderr, "%sProcedure call used as an expression\n", user), Putl(output, 1);
  break ;
   case eidkind:
  (void)fprintf(stderr, "%sIdentifier used in an invalid context\n", user), Putl(output, 1);
  break ;
   case etypeexpected:
  (void)fprintf(stderr, "%sType identifier expected\n", user), Putl(output, 1);
  break ;
   case econstexpected:
  (void)fprintf(stderr, "%sConstant expected\n", user), Putl(output, 1);
  break ;
   case ecalltarget:
  (void)fprintf(stderr, "%sCalled identifier is not a subroutine\n", user), Putl(output, 1);
  break ;
   case efuncstmt:
  (void)fprintf(stderr, "%sFunction call used as a statement\n", user), Putl(output, 1);
  break ;
   case ebadrange:
  (void)fprintf(stderr, "%sInvalid subrange bounds\n", user), Putl(output, 1);
  break ;
   default:
  Caseerror(Line);
 }
 if (lastline != 0) {
  (void)fprintf(stderr, "Line %1d, col %1d:\n", lastline, lastcol), Putl(output, 1);
  if (Member((unsigned)(m), Conset[0])) {
   i = 1;
   while ((i < xtoklen) && (lasttok.A[i - 1] != null)) {
    xtok.A[i - 1] = lasttok.A[i - 1];
    i = i + 1;
   }
   while (i < xtoklen) {
    xtok.A[i - 1] = ' ';
    i = i + 1;
   }
   xtok.A[xtoklen - 1] = ' ';
   (void)fprintf(stderr, "Current symbol: %.64s\n", xtok.A), Putl(output, 1);
  }
 }
# undef xtoklen
}

void fatal();

void error();

 char
uppercase(c)
 char c;
{
 register char R79;

 if ((c >= 'a') && (c <= 'z'))
  R79 = (unsigned)(c) + (unsigned)('A') - (unsigned)('a');
 else
  R79 = c;
 return R79;
}

 char
lowercase(c)
 char c;
{
 register char R80;

 if ((c >= 'A') && (c <= 'Z'))
  R80 = (unsigned)(c) - (unsigned)('A') + (unsigned)('a');
 else
  R80 = c;
 return R80;
}

 void
gettokn(i, t)
 strindx i;
 toknbuf *t;
{
 char c;
 toknidx k;
 strbidx j;
 strptr p;

 k = 1;
 p = strstor.A[i / (maxstrblk + 1)];
 j = i % (maxstrblk + 1);
 do {
  c = p->A[j];
  t->A[k - 1] = c;
  j = j + 1;
  k = k + 1;
  if (k == maxtoknlen) {
   c = null;
   t->A[maxtoknlen - 1] = null;
   prtmsg(eoverflow);
  }
 } while (!(c == null));
}

 void
puttokn(i, t)
 strindx i;
 toknbuf *t;
{
 char c;
 toknidx k;
 strbidx j;
 strptr p;

 k = 1;
 p = strstor.A[i / (maxstrblk + 1)];
 j = i % (maxstrblk + 1);
 do {
  c = t->A[k - 1];
  p->A[j] = c;
  k = k + 1;
  j = j + 1;
 } while (!(c == null));
}

 void
writetok(w)
 toknbuf *w;
{
 toknidx j;

 j = 1;
 while (w->A[j - 1] != null) {
  Putchr(w->A[j - 1], output);
  j = j + 1;
 }
}

 void
printtok(i)
 strindx i;
{
 toknbuf w;

 gettokn(i, &w);
 writetok(&w);
}

 void
printid(ip)
 idptr ip;
{
 printtok(ip->istr);
}

 void
printchr(c)
 char c;
{
 if ((c == quote) || (c == bslash))
  (void)fprintf(output.fp, "%c%c%c%c", quote, bslash, c, quote), Putl(output, 0);
 else
  (void)fprintf(output.fp, "%c%c%c", quote, c, quote), Putl(output, 0);
}

 void
printstr(i)
 strindx i;
{
 toknidx k;
 char c;
 toknbuf w;

 gettokn(i, &w);
 Putchr(cite, output);
 k = 1;
 while (w.A[k - 1] != null) {
  c = w.A[k - 1];
  k = k + 1;
  if ((c == cite) || (c == bslash))
   Putchr(bslash, output);
  Putchr(c, output);
 }
 Putchr(cite, output);
}

 treeptr
idup(ip)
 treeptr ip;
{
 register treeptr R81;

 R81 = ip->U.V43.tsym->lsymdecl->tup;
 return R81;
}

 hashtyp
hashtokn(id)
 toknbuf *id;
{
 register hashtyp R82;
 integer h;
 toknidx i;

 i = 1;
 h = 0;
 while (id->A[i - 1] != null) {
  h = h + (unsigned)(id->A[i - 1]);
  i = i + 1;
 }
 R82 = h % hashmax;
 return R82;
}

 strindx
savestr(t)
 toknbuf *t;
{
 register strindx R83;
 toknidx k;
 strindx i;
 strbcnt j;

 k = 1;
 while (t->A[k - 1] != null)
  k = k + 1;
 if (k > strleft) {
  if (strstor.A[maxblkcnt] != (strblk *)NIL)
   error(emanytokn);
  j = (strfree + maxstrblk) / (maxstrblk + 1);
  strstor.A[j] = (strblk *)malloc((unsigned)(sizeof(*strstor.A[j])));
  if (strstor.A[j] == (strblk *)NIL)
   error(enew);
  strfree = j * (maxstrblk + 1);
  strleft = maxstrblk;
 }
 i = strfree;
 strfree = strfree + k;
 strleft = strleft - k;
 puttokn(i, &(*t));
 R83 = i;
 return R83;
}

 idptr
saveid(id)
 toknbuf *id;
{
 register idptr R84;
 toknidx k;
 idptr ip;
 hashtyp h;
 toknbuf t;

 h = hashtokn(&(*id));
 ip = idtab.A[h];
 while (ip != (struct S62 *)NIL) {
  gettokn(ip->istr, &t);
  k = 1;
  while (id->A[k - 1] == t.A[k - 1])
   if (id->A[k - 1] == null)
    goto L999;
   else
    k = k + 1;
  ip = ip->inext;
 }
 ip = (struct S62 *)malloc((unsigned)(sizeof(*ip)));
 if (ip == (struct S62 *)NIL)
  error(enew);
 ip->inref = 0;
 ip->istr = savestr(&(*id));
 ip->ihash = h;
 ip->inext = idtab.A[h];
 idtab.A[h] = ip;
L999:
 R84 = ip;
 return R84;
}

 idptr
mkconc(sep, p, q)
 char sep;
 idptr p, q;
{
 register idptr R85;
 toknbuf w, x;
 toknidx i, j;

 gettokn(q->istr, &x);
 j = 1;
 while (x.A[j - 1] != null)
  j = j + 1;
 w.A[1 - 1] = null;
 if (p != (struct S62 *)NIL)
  gettokn(p->istr, &w);
 i = 1;
 while (w.A[i - 1] != null)
  i = i + 1;
 if (i + j + 2 >= maxtoknlen)
  error(eoverflow);
 if (sep == '>') {
  w.A[i - 1] = '-';
  i = i + 1;
 }
 if (sep != space) {
  w.A[i - 1] = sep;
  i = i + 1;
 }
 j = 1;
 do {
  w.A[i - 1] = x.A[j - 1];
  i = i + 1;
  j = j + 1;
 } while (!(w.A[i - 1 - 1] == null));
 R85 = saveid(&w);
 return R85;
}

idptr mkuniqname();

 void
dig(n)
 integer n;
{
 if (n > 0) {
  dig(n / 10);
  if ((*G204_i) == maxtoknlen)
   error(eoverflow);
  G202_t->A[(*G204_i) - 1] = n % 10 + (unsigned)('0');
  (*G204_i) = (*G204_i) + 1;
 }
}

 idptr
mkuniqname(t)
 toknbuf *t;
{
 register idptr R86;
 toknidx i;
 toknbuf *F203;
 toknidx *F205;

 F205 = G204_i;
 G204_i = &i;
 F203 = G202_t;
 G202_t = &(*t);
 (*G204_i) = 1;
 while (G202_t->A[(*G204_i) - 1] != null)
  (*G204_i) = (*G204_i) + 1;
 varno = varno + 1;
 dig(varno);
 G202_t->A[(*G204_i) - 1] = null;
 R86 = saveid(&(*G202_t));
 G202_t = F203;
 G204_i = F205;
 return R86;
}

 idptr
mkvariable(c)
 char c;
{
 register idptr R87;
 toknbuf t;

 t.A[1 - 1] = c;
 t.A[2 - 1] = null;
 R87 = mkuniqname(&t);
 return R87;
}

 idptr
mkrename(c, ip)
 char c;
 idptr ip;
{
 register idptr R88;

 R88 = mkconc(uscore, mkvariable(c), ip);
 return R88;
}

 idptr
mkvrnt()
{
 register idptr R89;
 toknbuf t;

 t.A[1 - 1] = 'U';
 t.A[2 - 1] = '.';
 t.A[3 - 1] = 'V';
 t.A[4 - 1] = null;
 R89 = mkuniqname(&t);
 return R89;
}

 void
checksymbol(ss)
 symset ss;
{
 if (!(Member((unsigned)(currsym.st), ss.S)))
  error(ebadsymbol);
}

void nextsymbol();

 char
nextchar()
{
 register char R90;
 char c;

 if (pushed) {
  c = pushchr;
  pushed = false;
 } else
  if (Eof(input))
   c = null;
  else {
   colno = colno + 1;
   if (Eoln(input)) {
    lineno = lineno + 1;
    colno = 0;
   }
   c = Getchr(input);
   if (echo)
    if (colno == 0)
     Putchr('\n', output);
    else
     Putchr(c, output);
   if ((unsigned)(c) == 9)
    colno = (((colno - 1) / tabwidth) + 1) * tabwidth;
  }
 if ((*G207_lastchr) > 0) {
  lasttok.A[(*G207_lastchr) - 1] = c;
  (*G207_lastchr) = (*G207_lastchr) + 1;
 }
 R90 = c;
 return R90;
}

 char
peekchar()
{
 register char R91;

 if (pushed)
  R91 = pushchr;
 else
  if (Eof(input))
   R91 = null;
  else
   R91 = input.buf;
 return R91;
}

void nexttoken();

 boolean
idchar(c)
 char c;
{
 register boolean R92;

 R92 = (boolean)((c >= 'a') && (c <= 'z') || (c >= '0') && (c <= '9') || (c >= 'A') && (c <= 'Z') || (c == uscore));
 return R92;
}

 boolean
numchar(c)
 char c;
{
 register boolean R93;

 R93 = (boolean)((c >= '0') && (c <= '9'));
 return R93;
}

 integer
numval(c)
 char c;
{
 register integer R94;

 R94 = (unsigned)(c) - (unsigned)('0');
 return R94;
}

 symtyp
keywordcheck(w, l)
 toknbuf *w;
 toknidx l;
{
 register symtyp R95;
 register unsigned char n;
 unsigned char i, j, k;
 keyword wrd;
 symtyp kwc;

 if ((l > 1) && (l < keywordlen)) {
  wrd = keytab.A[keytablen].wrd;
  {
   unsigned char B44 = 1,
    B45 = l;

   if (B44 <= B45)
    for (n = B44; ; n++) {
     wrd.A[n - 1] = w->A[n - 1];
     if (n == B45) break;
    }
  }
  i = 0;
  j = keytablen;
  while (j > i) {
   k = (i + j) / 2;
   if (Cmpstr(keytab.A[k].wrd.A, wrd.A) >= 0)
    j = k;
   else
    i = k + 1;
  }
  if (Cmpstr(keytab.A[j].wrd.A, wrd.A) == 0)
   kwc = keytab.A[j].sym;
  else
   kwc = sid;
 } else
  kwc = sid;
 R95 = kwc;
 return R95;
}

 void
nexttoken(realok)
 boolean realok;
{
 char c;
 integer n;
 boolean ready;
 toknidx wl;
 toknbuf wb;

 (*G207_lastchr) = 0;
 do {
  c = nextchar();
  if (c == '{') {
   do {
    c = nextchar();
    if (diffcomm)
     ready = (boolean)(c == '}');
    else
     ready = (boolean)(((c == '*') && (peekchar() == ')')) || (c == '}'));
   } while (!(ready || Eof(input)));
   if (Eof(input) && !ready)
    error(eeofcmnt);
   if ((c == '*') && !Eof(input))
    c = nextchar();
   c = space;
  } else
   if ((c == '(') && (peekchar() == '*')) {
    c = nextchar();
    do {
     c = nextchar();
     if (diffcomm)
      ready = (boolean)((c == '*') && (peekchar() == ')'));
     else
      ready = (boolean)(((c == '*') && (peekchar() == ')')) || (c == '}'));
    } while (!(ready || Eof(input)));
    if (Eof(input) && !ready)
     error(eeofcmnt);
    if ((c == '*') && !Eof(input))
     c = nextchar();
    c = space;
   }
 } while (!((c != space) && ((unsigned)(c) != 9)));
 lasttok.A[1 - 1] = c;
 (*G207_lastchr) = 2;
 lastcol = colno;
 lastline = lineno;
 if (c < okchr)
  c = badchr;
 {
  register struct S206 *W46 = &currsym;

  if (Eof(input)) {
   lasttok.A[1 - 1] = '*';
   lasttok.A[2 - 1] = 'E';
   lasttok.A[3 - 1] = 'O';
   lasttok.A[4 - 1] = 'F';
   lasttok.A[5 - 1] = '*';
   (*G207_lastchr) = 6;
   W46->st = seof;
  } else
   switch ((int)(c)) {
     case '|':  case '`':  case '~':  case '}':
     case 92:  case 95:  case 63:
    error(ebadchar);
    break ;
     case 'a':  case 'b':  case 'c':  case 'd':
     case 'e':  case 'f':  case 'g':  case 'h':
     case 'i':  case 'j':  case 'k':  case 'l':
     case 'm':  case 'n':  case 'o':  case 'p':
     case 'q':  case 'r':  case 's':  case 't':
     case 'u':  case 'v':  case 'w':  case 'x':
     case 'y':  case 'z':  case 'A':  case 'B':
     case 'C':  case 'D':  case 'E':  case 'F':
     case 'G':  case 'H':  case 'I':  case 'J':
     case 'K':  case 'L':  case 'M':  case 'N':
     case 'O':  case 'P':  case 'Q':  case 'R':
     case 'S':  case 'T':  case 'U':  case 'V':
     case 'W':  case 'X':  case 'Y':  case 'Z':
    wb.A[1 - 1] = lowercase(c);
    wl = 2;
    while ((wl < maxtoknlen) && idchar(peekchar())) {
     wb.A[wl - 1] = lowercase(nextchar());
     wl = wl + 1;
    }
    if (wl >= maxtoknlen) {
     lasttok.A[(*G207_lastchr) - 1] = null;
     error(elongtokn);
    }
    wb.A[wl - 1] = null;
    W46->st = keywordcheck(&wb, wl - 1);
    if (W46->st == sid)
     W46->U.V1.vid = saveid(&wb);
    break ;
     case '0':  case '1':  case '2':  case '3':
     case '4':  case '5':  case '6':  case '7':
     case '8':  case '9':
    wb.A[1 - 1] = c;
    wl = 2;
    n = numval(c);
    while (numchar(peekchar())) {
     c = nextchar();
     n = n * 10 + numval(c);
     wb.A[wl - 1] = c;
     wl = wl + 1;
    }
    W46->st = sinteger;
    W46->U.V3.vint = n;
    if (realok && (peekchar() == '.')) {
     c = nextchar();
     realok = numchar(peekchar());
     pushchr = c;
     pushed = true;
    }
    if (realok) {
     if (peekchar() == '.') {
      W46->st = sreal;
      wb.A[wl - 1] = nextchar();
      wl = wl + 1;
      while (numchar(peekchar())) {
       wb.A[wl - 1] = nextchar();
       wl = wl + 1;
      }
     }
     c = peekchar();
     if ((c == 'e') || (c == 'E')) {
      W46->st = sreal;
      c = nextchar();
      wb.A[wl - 1] = xpnent;
      wl = wl + 1;
      c = peekchar();
      if ((c == '-') || (c == '+')) {
       wb.A[wl - 1] = nextchar();
       wl = wl + 1;
      }
      while (numchar(peekchar())) {
       wb.A[wl - 1] = nextchar();
       wl = wl + 1;
      }
     }
     if (W46->st == sreal) {
      wb.A[wl - 1] = null;
      W46->U.V4.vflt = savestr(&wb);
     }
    }
    break ;
     case '(':
    if (peekchar() == '.') {
     c = nextchar();
     W46->st = slbrack;
    } else
     W46->st = slpar;
    break ;
     case ')':
    W46->st = srpar;
    break ;
     case '[':
    W46->st = slbrack;
    break ;
     case ']':
    W46->st = srbrack;
    break ;
     case '.':
    if (peekchar() == '.') {
     c = nextchar();
     W46->st = sdotdot;
    } else
     if (peekchar() == ')') {
      c = nextchar();
      W46->st = srbrack;
     } else
      W46->st = sdot;
    break ;
     case ';':
    W46->st = ssemic;
    break ;
     case ':':
    if (peekchar() == '=') {
     c = nextchar();
     W46->st = sassign;
    } else
     W46->st = scolon;
    break ;
     case ',':
    W46->st = scomma;
    break ;
     case '@':  case '^':
    W46->st = sarrow;
    break ;
     case '=':
    W46->st = seq;
    break ;
     case '<':
    if (peekchar() == '=') {
     c = nextchar();
     W46->st = sle;
    } else
     if (peekchar() == '>') {
      c = nextchar();
      W46->st = sne;
     } else
      W46->st = slt;
    break ;
     case '>':
    if (peekchar() == '=') {
     c = nextchar();
     W46->st = sge;
    } else
     W46->st = sgt;
    break ;
     case '+':
    W46->st = splus;
    break ;
     case '-':
    W46->st = sminus;
    break ;
     case '*':
    W46->st = smul;
    break ;
     case '/':
    W46->st = squot;
    break ;
     case 39:
    wl = 1;
    ready = false;
    do {
     if (Eoln(input)) {
      lasttok.A[(*G207_lastchr) - 1] = null;
      error(ebadstring);
     }
     c = nextchar();
     if (c == quote)
      if (peekchar() == quote)
       c = nextchar();
      else
       ready = true;
     if (c == null) {
      if (Eof(input))
       error(eeofstr);
      lasttok.A[(*G207_lastchr) - 1] = null;
      error(enulchr);
     }
     if (!ready) {
      wb.A[wl - 1] = c;
      if (wl >= maxtoknlen) {
       lasttok.A[(*G207_lastchr) - 1] = null;
       error(elongstring);
      }
      wl = wl + 1;
     }
    } while (!(ready));
    if (wl == 2) {
     W46->st = schar;
     W46->U.V2.vchr = wb.A[1 - 1];
    } else {
     wb.A[wl - 1] = null;
     W46->st = sstring;
     W46->U.V5.vstr = savestr(&wb);
    }
    break ;
     default:
    Caseerror(Line);
   }
 }
 if ((*G207_lastchr) == 0)
  (*G207_lastchr) = 1;
 lasttok.A[(*G207_lastchr) - 1] = null;
}

 void
nextsymbol(ss)
 symset ss;
{
 unsigned char lastchr;
 unsigned char *F208;

 F208 = G207_lastchr;
 G207_lastchr = &lastchr;
 nexttoken((boolean)(Member((unsigned)(sreal), ss.S)));
 checksymbol(ss);
 G207_lastchr = F208;
}

 treeptr
typeof(tp)
 treeptr tp;
{
 register treeptr R96;
 treeptr tf, tq;

 tq = tp;
 tf = tq->ttype;
 while (tf == (struct S64 *)NIL) {
  switch ((int)(tq->tt)) {
    case nchar:
   tf = typnods.A[(int)(tchar)];
   break ;
    case ninteger:
   tf = typnods.A[(int)(tinteger)];
   break ;
    case nreal:
   tf = typnods.A[(int)(treal)];
   break ;
    case nstring:
   tf = typnods.A[(int)(tstring)];
   break ;
    case nnil:
   tf = typnods.A[(int)(tnil)];
   break ;
    case nid:
   tq = idup(tq);
   if (tq == (struct S64 *)NIL)
    fatal(etree);
   break ;
    case ntype:  case nvar:  case nconst:  case nfield:
    case nvalpar:  case nvarpar:
   tq = tq->U.V14.tbind;
   break ;
    case npredef:  case nptr:  case nscalar:  case nrecord:
    case nconfarr:  case narray:  case nfileof:  case nsetof:
   tf = tq;
   break ;
    case nsubrange:
   if (tq->tup->tt == nconfarr)
    tf = tq->tup->U.V22.tindtyp;
   else
    tf = tq;
   break ;
    case ncall:
   tf = typeof(tq->U.V30.tcall);
   if (tf == typnods.A[(int)(tpoly)])
    tf = typeof(tq->U.V30.taparm);
   break ;
    case nfunc:
   tq = tq->U.V13.tfuntyp;
   break ;
    case nparfunc:
   tq = tq->U.V15.tpartyp;
   break ;
    case nproc:  case nparproc:
   tf = typnods.A[(int)(tnone)];
   break ;
    case nvariant:  case nlabel:  case npgm:  case nempty:
    case nbegin:  case nlabstmt:  case nassign:  case npush:
    case npop:  case nif:  case nwhile:  case nrepeat:
    case nfor:  case ncase:  case nchoise:  case ngoto:
    case nwith:  case nwithvar:
   fatal(etree);
   break ;
    case nformat:  case nrange:
   tq = tq->U.V41.texpl;
   break ;
    case nplus:  case nminus:  case nmul:
   tf = typeof(tq->U.V41.texpl);
   if (tf == typnods.A[(int)(tinteger)])
    tf = typeof(tq->U.V41.texpr);
   else
    if (tf->tt == nsetof)
     tf = typnods.A[(int)(tset)];
   break ;
    case numinus:  case nuplus:
   tq = tq->U.V42.texps;
   break ;
    case nmod:  case ndiv:
   tf = typnods.A[(int)(tinteger)];
   break ;
    case nquot:
   tf = typnods.A[(int)(treal)];
   break ;
    case neq:  case nne:  case nlt:  case nle:
    case ngt:  case nge:  case nin:  case nor:
    case nand:  case nnot:
   tf = typnods.A[(int)(tboolean)];
   break ;
    case nset:
   tf = typnods.A[(int)(tset)];
   break ;
    case nselect:
   tq = tq->U.V40.tfield;
   break ;
    case nderef:
   tq = typeof(tq->U.V42.texps);
   switch ((int)(tq->tt)) {
     case nptr:
    tq = tq->U.V16.tptrid;
    break ;
     case nfileof:
    tq = tq->U.V18.tof;
    break ;
     case npredef:
    tf = typnods.A[(int)(tchar)];
    break ;
     default:
    Caseerror(Line);
   }
   break ;
    case nindex:
   tq = typeof(tq->U.V39.tvariable);
   if (tq->tt == nconfarr)
    tq = tq->U.V22.tcelem;
   else
    if (tq == typnods.A[(int)(tstring)])
     tf = typnods.A[(int)(tchar)];
    else
     tq = tq->U.V23.taelem;
   break ;
    default:
   Caseerror(Line);
  }
 }
 if (tp->ttype == (struct S64 *)NIL)
  tp->ttype = tf;
 R96 = tf;
 return R96;
}

 void
linkup(up, tp)
 treeptr up, tp;
{
 while (tp != (struct S64 *)NIL) {
  if (tp->tup == (struct S64 *)NIL) {
   tp->tup = up;
   switch ((int)(tp->tt)) {
     case npgm:  case nfunc:  case nproc:
    linkup(tp, tp->U.V13.tsubid);
    linkup(tp, tp->U.V13.tsubpar);
    linkup(tp, tp->U.V13.tfuntyp);
    linkup(tp, tp->U.V13.tsublab);
    linkup(tp, tp->U.V13.tsubconst);
    linkup(tp, tp->U.V13.tsubtype);
    linkup(tp, tp->U.V13.tsubvar);
    linkup(tp, tp->U.V13.tsubsub);
    linkup(tp, tp->U.V13.tsubstmt);
    break ;
     case nvalpar:  case nvarpar:  case nconst:  case ntype:
     case nfield:  case nvar:
    linkup(tp, tp->U.V14.tidl);
    linkup(tp, tp->U.V14.tbind);
    break ;
     case nparproc:  case nparfunc:
    linkup(tp, tp->U.V15.tparid);
    linkup(tp, tp->U.V15.tparparm);
    linkup(tp, tp->U.V15.tpartyp);
    break ;
     case nptr:
    linkup(tp, tp->U.V16.tptrid);
    break ;
     case nscalar:
    linkup(tp, tp->U.V17.tscalid);
    break ;
     case nsubrange:
    linkup(tp, tp->U.V19.tlo);
    linkup(tp, tp->U.V19.thi);
    break ;
     case nvariant:
    linkup(tp, tp->U.V20.tselct);
    linkup(tp, tp->U.V20.tvrnt);
    break ;
     case nrecord:
    linkup(tp, tp->U.V21.tflist);
    linkup(tp, tp->U.V21.tvlist);
    break ;
     case nconfarr:
    linkup(tp, tp->U.V22.tcindx);
    linkup(tp, tp->U.V22.tcelem);
    linkup(tp, tp->U.V22.tindtyp);
    break ;
     case narray:
    linkup(tp, tp->U.V23.taindx);
    linkup(tp, tp->U.V23.taelem);
    break ;
     case nfileof:  case nsetof:
    linkup(tp, tp->U.V18.tof);
    break ;
     case nbegin:
    linkup(tp, tp->U.V24.tbegin);
    break ;
     case nlabstmt:
    linkup(tp, tp->U.V25.tlabno);
    linkup(tp, tp->U.V25.tstmt);
    break ;
     case nassign:
    linkup(tp, tp->U.V27.tlhs);
    linkup(tp, tp->U.V27.trhs);
    break ;
     case npush:  case npop:
    linkup(tp, tp->U.V28.tglob);
    linkup(tp, tp->U.V28.tloc);
    linkup(tp, tp->U.V28.ttmp);
    break ;
     case ncall:
    linkup(tp, tp->U.V30.tcall);
    linkup(tp, tp->U.V30.taparm);
    break ;
     case nif:
    linkup(tp, tp->U.V31.tifxp);
    linkup(tp, tp->U.V31.tthen);
    linkup(tp, tp->U.V31.telse);
    break ;
     case nwhile:
    linkup(tp, tp->U.V32.twhixp);
    linkup(tp, tp->U.V32.twhistmt);
    break ;
     case nrepeat:
    linkup(tp, tp->U.V33.treptstmt);
    linkup(tp, tp->U.V33.treptxp);
    break ;
     case nfor:
    linkup(tp, tp->U.V34.tforid);
    linkup(tp, tp->U.V34.tfrom);
    linkup(tp, tp->U.V34.tto);
    linkup(tp, tp->U.V34.tforstmt);
    break ;
     case ncase:
    linkup(tp, tp->U.V35.tcasxp);
    linkup(tp, tp->U.V35.tcaslst);
    linkup(tp, tp->U.V35.tcasother);
    break ;
     case nchoise:
    linkup(tp, tp->U.V36.tchocon);
    linkup(tp, tp->U.V36.tchostmt);
    break ;
     case nwith:
    linkup(tp, tp->U.V37.twithvar);
    linkup(tp, tp->U.V37.twithstmt);
    break ;
     case nwithvar:
    linkup(tp, tp->U.V38.texpw);
    break ;
     case nindex:
    linkup(tp, tp->U.V39.tvariable);
    linkup(tp, tp->U.V39.toffset);
    break ;
     case nselect:
    linkup(tp, tp->U.V40.trecord);
    linkup(tp, tp->U.V40.tfield);
    break ;
     case ngoto:
    linkup(tp, tp->U.V26.tlabel);
    break ;
     case nrange:  case nformat:  case nin:  case neq:
     case nne:  case nlt:  case nle:  case ngt:
     case nge:  case nor:  case nplus:  case nminus:
     case nand:  case nmul:  case ndiv:  case nmod:
     case nquot:
    linkup(tp, tp->U.V41.texpl);
    linkup(tp, tp->U.V41.texpr);
    break ;
     case nderef:  case nnot:  case nset:  case numinus:
     case nuplus:
    linkup(tp, tp->U.V42.texps);
    break ;
     case nid:  case nnil:  case ninteger:  case nreal:
     case nchar:  case nstring:  case npredef:  case nlabel:
     case nempty:
    break ;
     default:
    Caseerror(Line);
   }
  }
  tp = tp->tnext;
 }
}

 symptr
mksym(vt)
 ltypes vt;
{
 register symptr R97;
 symptr mp;

 mp = (struct S65 *)malloc((unsigned)(sizeof(*mp)));
 if (mp == (struct S65 *)NIL)
  error(enew);
 mp->lt = vt;
 mp->lnext = (struct S65 *)NIL;
 mp->lsymdecl = (struct S64 *)NIL;
 mp->ldecl = (struct S63 *)NIL;
 R97 = mp;
 return R97;
}

 void
declsym(sp)
 symptr sp;
{
 hashtyp h;

 if (Member((unsigned)(sp->lt), Conset[1]))
  h = sp->U.V6.lid->ihash;
 else
  h = hashmax;
 sp->lnext = symtab->ddecl.A[h];
 symtab->ddecl.A[h] = sp;
 sp->ldecl = symtab;
}

 treeptr
mknode(nt)
 treetyp nt;
{
 register treeptr R98;
 treeptr tp;

 tp = (struct S64 *)NIL;
 switch ((int)(nt)) {
   case npredef:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V12.tdef) + sizeof(tp->U.V12)));
  break ;
   case npgm:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V13.tsubid) + sizeof(tp->U.V13)));
  break ;
   case nfunc:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V13.tsubid) + sizeof(tp->U.V13)));
  break ;
   case nproc:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V13.tsubid) + sizeof(tp->U.V13)));
  break ;
   case nlabel:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V43.tsym) + sizeof(tp->U.V43)));
  break ;
   case nconst:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V14.tidl) + sizeof(tp->U.V14)));
  break ;
   case ntype:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V14.tidl) + sizeof(tp->U.V14)));
  break ;
   case nvar:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V14.tidl) + sizeof(tp->U.V14)));
  break ;
   case nvalpar:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V14.tidl) + sizeof(tp->U.V14)));
  break ;
   case nvarpar:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V14.tidl) + sizeof(tp->U.V14)));
  break ;
   case nparproc:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V15.tparid) + sizeof(tp->U.V15)));
  break ;
   case nparfunc:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V15.tparid) + sizeof(tp->U.V15)));
  break ;
   case nsubrange:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V19.tlo) + sizeof(tp->U.V19)));
  break ;
   case nvariant:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V20.tselct) + sizeof(tp->U.V20)));
  break ;
   case nfield:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V14.tidl) + sizeof(tp->U.V14)));
  break ;
   case nrecord:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V21.tflist) + sizeof(tp->U.V21)));
  break ;
   case nconfarr:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V22.tcindx) + sizeof(tp->U.V22)));
  break ;
   case narray:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V23.taindx) + sizeof(tp->U.V23)));
  break ;
   case nfileof:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V18.tof) + sizeof(tp->U.V18)));
  break ;
   case nsetof:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V18.tof) + sizeof(tp->U.V18)));
  break ;
   case nbegin:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V24.tbegin) + sizeof(tp->U.V24)));
  break ;
   case nptr:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V16.tptrid) + sizeof(tp->U.V16)));
  break ;
   case nscalar:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V17.tscalid) + sizeof(tp->U.V17)));
  break ;
   case nif:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V31.tifxp) + sizeof(tp->U.V31)));
  break ;
   case nwhile:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V32.twhixp) + sizeof(tp->U.V32)));
  break ;
   case nrepeat:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V33.treptstmt) + sizeof(tp->U.V33)));
  break ;
   case nfor:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V34.tforid) + sizeof(tp->U.V34)));
  break ;
   case ncase:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V35.tcasxp) + sizeof(tp->U.V35)));
  break ;
   case nchoise:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V36.tchocon) + sizeof(tp->U.V36)));
  break ;
   case ngoto:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V26.tlabel) + sizeof(tp->U.V26)));
  break ;
   case nwith:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V37.twithvar) + sizeof(tp->U.V37)));
  break ;
   case nwithvar:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V38.texpw) + sizeof(tp->U.V38)));
  break ;
   case nempty:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V12.tdef)));
  break ;
   case nlabstmt:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V25.tlabno) + sizeof(tp->U.V25)));
  break ;
   case nassign:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V27.tlhs) + sizeof(tp->U.V27)));
  break ;
   case nformat:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nin:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case neq:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nne:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nlt:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nle:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case ngt:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nge:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nor:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nplus:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nminus:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nand:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nmul:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case ndiv:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nmod:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nquot:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nnot:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V42.texps) + sizeof(tp->U.V42)));
  break ;
   case numinus:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V42.texps) + sizeof(tp->U.V42)));
  break ;
   case nuplus:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V42.texps) + sizeof(tp->U.V42)));
  break ;
   case nset:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V42.texps) + sizeof(tp->U.V42)));
  break ;
   case nrange:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V41.texpl) + sizeof(tp->U.V41)));
  break ;
   case nindex:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V39.tvariable) + sizeof(tp->U.V39)));
  break ;
   case nselect:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V40.trecord) + sizeof(tp->U.V40)));
  break ;
   case nderef:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V42.texps) + sizeof(tp->U.V42)));
  break ;
   case ncall:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V30.tcall) + sizeof(tp->U.V30)));
  break ;
   case nid:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V43.tsym) + sizeof(tp->U.V43)));
  break ;
   case nchar:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V43.tsym) + sizeof(tp->U.V43)));
  break ;
   case ninteger:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V43.tsym) + sizeof(tp->U.V43)));
  break ;
   case nreal:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V43.tsym) + sizeof(tp->U.V43)));
  break ;
   case nstring:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V43.tsym) + sizeof(tp->U.V43)));
  break ;
   case nnil:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V12.tdef)));
  break ;
   case npush:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V28.tglob) + sizeof(tp->U.V28)));
  break ;
   case npop:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V28.tglob) + sizeof(tp->U.V28)));
  break ;
   case nbreak:
  tp = (struct S64 *)malloc((unsigned)(Unionoffs(tp, U.V29.tbrkid) + sizeof(tp->U.V29)));
  break ;
   default:
  Caseerror(Line);
 }
 if (tp == (struct S64 *)NIL)
  error(enew);
 tp->tt = nt;
 tp->tnext = (struct S64 *)NIL;
 tp->tup = (struct S64 *)NIL;
 tp->ttype = (struct S64 *)NIL;
 tp->tline = lastline;
 tp->tcol = lastcol;
 tp->tsemchecked = false;
 tp->tsemchecking = false;
 if (Member((unsigned)(nt), Conset[2]))
  tp->U.V42.tuplusop = false;
 R98 = tp;
 return R98;
}

 treeptr
mklit()
{
 register treeptr R99;
 symptr sp;
 treeptr tp;

 switch ((int)(currsym.st)) {
   case sinteger:
  sp = mksym(linteger);
  sp->U.V10.linum = currsym.U.V3.vint;
  tp = mknode(ninteger);
  break ;
   case sreal:
  sp = mksym(lreal);
  sp->U.V8.lfloat = currsym.U.V4.vflt;
  tp = mknode(nreal);
  break ;
   case schar:
  sp = mksym(lcharacter);
  sp->U.V11.lchar = currsym.U.V2.vchr;
  tp = mknode(nchar);
  break ;
   case sstring:
  sp = mksym(lstring);
  sp->U.V7.lstr = currsym.U.V5.vstr;
  tp = mknode(nstring);
  break ;
   default:
  Caseerror(Line);
 }
 tp->U.V43.tsym = sp;
 sp->lsymdecl = tp;
 R99 = tp;
 return R99;
}

 symptr
lookupid(ip, fieldok)
 idptr ip;
 boolean fieldok;
{
 register symptr R100;
 symptr sp;
 declptr dp;
 struct { setword S[2]; } vs;

 R100 = (struct S65 *)NIL;
 if (fieldok)
  Setncpy(vs.S, Conset[3], sizeof(vs.S));
 else
  Setncpy(vs.S, Conset[4], sizeof(vs.S));
 sp = (struct S65 *)NIL;
 dp = symtab;
 while (dp != (struct S63 *)NIL) {
  sp = dp->ddecl.A[ip->ihash];
  while (sp != (struct S65 *)NIL) {
   if ((Member((unsigned)(sp->lt), vs.S)) && (sp->U.V6.lid == ip))
    goto L999;
   sp = sp->lnext;
  }
  dp = dp->dprev;
 }
L999:
 R100 = sp;
 return R100;
}

 symptr
lookuplabel(i)
 integer i;
{
 register symptr R101;
 symptr sp;
 declptr dp;

 sp = (struct S65 *)NIL;
 dp = symtab;
 while (dp != (struct S63 *)NIL) {
  sp = dp->ddecl.A[hashmax];
  while (sp != (struct S65 *)NIL) {
   if ((Member((unsigned)(sp->lt), Conset[5])) && (sp->U.V9.lno == i))
    goto L999;
   sp = sp->lnext;
  }
  dp = dp->dprev;
 }
L999:
 R101 = sp;
 return R101;
}

 void
enterscope(dp)
 declptr dp;
{
 register hashtyp h;

 if (dp == (struct S63 *)NIL) {
  dp = (struct S63 *)malloc((unsigned)(sizeof(*dp)));
  {
   hashtyp B47 = 0,
    B48 = hashmax;

   if (B47 <= B48)
    for (h = B47; ; h++) {
     dp->ddecl.A[h] = (struct S65 *)NIL;
     if (h == B48) break;
    }
  }
 }
 dp->dprev = symtab;
 symtab = dp;
}

 declptr
currscope()
{
 register declptr R102;

 R102 = symtab;
 return R102;
}

 void
leavescope()
{
 symtab = symtab->dprev;
}

 symptr
mkid(ip)
 idptr ip;
{
 register symptr R103;
 symptr sp;

 sp = mksym(lidentifier);
 sp->U.V6.lid = ip;
 sp->U.V6.lused = false;
 declsym(sp);
 ip->inref = ip->inref + 1;
 R103 = sp;
 return R103;
}

 treeptr
newid(ip)
 idptr ip;
{
 register treeptr R104;
 symptr sp;
 treeptr tp;

 sp = lookupid(ip, false);
 if (sp != (struct S65 *)NIL)
  if (sp->ldecl != symtab)
   sp = (struct S65 *)NIL;
 if (sp == (struct S65 *)NIL) {
  tp = mknode(nid);
  sp = mkid(ip);
  sp->lsymdecl = tp;
  tp->U.V43.tsym = sp;
 } else
  if (sp->lt == lpointer) {
   tp = mknode(nid);
   tp->U.V43.tsym = sp;
   sp->lt = lidentifier;
   sp->lsymdecl = tp;
  } else
   if (sp->lt == lforward) {
    sp->lt = lidentifier;
    tp = sp->lsymdecl;
   } else
    error(emultdeclid);
 R104 = tp;
 return R104;
}

 treeptr
oldid(ip, l)
 idptr ip;
 ltypes l;
{
 register treeptr R105;
 symptr sp;
 treeptr tp;

 sp = lookupid(ip, true);
 if (sp == (struct S65 *)NIL) {
  if (Member((unsigned)(l), Conset[6])) {
   tp = newid(ip);
   tp->U.V43.tsym->lt = l;
  } else
   error(enotdeclid);
 } else {
  sp->U.V6.lused = true;
  tp = mknode(nid);
  tp->U.V43.tsym = sp;
  if ((sp->lt == lpointer) && (l == lidentifier)) {
   sp->lt = lidentifier;
   sp->lsymdecl = tp;
  }
 }
 R105 = tp;
 return R105;
}

 treeptr
oldfield(tp, ip)
 treeptr tp;
 idptr ip;
{
 register treeptr R106;
 treeptr tq, ti, fp;

 fp = (struct S64 *)NIL;
 tq = tp->U.V21.tflist;
 while (tq != (struct S64 *)NIL) {
  ti = tq->U.V14.tidl;
  while (ti != (struct S64 *)NIL) {
   if (ti->U.V43.tsym->U.V6.lid == ip) {
    fp = mknode(nid);
    fp->U.V43.tsym = ti->U.V43.tsym;
    goto L999;
   }
   ti = ti->tnext;
  }
  tq = tq->tnext;
 }
 tq = tp->U.V21.tvlist;
 while (tq != (struct S64 *)NIL) {
  fp = oldfield(tq->U.V20.tvrnt, ip);
  if (fp != (struct S64 *)NIL)
   tq = (struct S64 *)NIL;
  else
   tq = tq->tnext;
 }
L999:
 R106 = fp;
 return R106;
}

void parse();

treeptr plabel();

treeptr pidlist();

treeptr pconst();

treeptr pconstant();

treeptr precord();

treeptr ptypedef();

treeptr ptype();

treeptr pvar();

treeptr psubs();

treeptr psubpar();

treeptr plabstmt();

treeptr pstmt();

treeptr psimple();

treeptr pvariable();

treeptr pexpr();

treeptr pcase();

treeptr pif();

treeptr pwhile();

treeptr prepeat();

treeptr pfor();

treeptr pwith();

treeptr pgoto();

treeptr pbegin();

void scopeup();

 void
addfields(rp)
 treeptr rp;
{
 treeptr fp, ip, vp;
 symptr sp;

 fp = rp->U.V21.tflist;
 while (fp != (struct S64 *)NIL) {
  ip = fp->U.V14.tidl;
  while (ip != (struct S64 *)NIL) {
   sp = mksym(lfield);
   sp->U.V6.lid = ip->U.V43.tsym->U.V6.lid;
   sp->U.V6.lused = false;
   sp->lsymdecl = ip;
   declsym(sp);
   ip = ip->tnext;
  }
  fp = fp->tnext;
 }
 vp = rp->U.V21.tvlist;
 while (vp != (struct S64 *)NIL) {
  addfields(vp->U.V20.tvrnt);
  vp = vp->tnext;
 }
}

 void
scopeup(tp)
 treeptr tp;
{
 addfields(typeof(tp));
}

 treeptr
newlbl()
{
 register treeptr R130;
 symptr sp;
 treeptr tp;

 tp = mknode(nlabel);
 sp = lookuplabel(currsym.U.V3.vint);
 if (sp != (struct S65 *)NIL)
  if (sp->ldecl != symtab)
   sp = (struct S65 *)NIL;
 if (sp == (struct S65 *)NIL) {
  sp = mksym(lforwlab);
  sp->U.V9.lno = currsym.U.V3.vint;
  sp->U.V9.lgo = false;
  sp->lsymdecl = tp;
  declsym(sp);
 } else
  error(emultdecllab);
 tp->U.V43.tsym = sp;
 R130 = tp;
 return R130;
}

 treeptr
oldlbl(defpt)
 boolean defpt;
{
 register treeptr R131;
 symptr sp;
 treeptr tp;

 sp = lookuplabel(currsym.U.V3.vint);
 if (sp == (struct S65 *)NIL) {
  prtmsg(enotdecllab);
  tp = newlbl();
  sp = tp->U.V43.tsym;
 } else {
  tp = mknode(nlabel);
  tp->U.V43.tsym = sp;
 }
 if (defpt) {
  if (sp->lt == lforwlab)
   sp->lt = llabel;
  else
   error(emuldeflab);
 }
 R131 = tp;
 return R131;
}

 void
pbody(tp)
 treeptr tp;
{
 treeptr tq;

 statlvl = statlvl + 1;
 if (currsym.st == slabel) {
  tp->U.V13.tsublab = plabel();
  linkup(tp, tp->U.V13.tsublab);
 } else
  tp->U.V13.tsublab = (struct S64 *)NIL;
 if (currsym.st == sconst) {
  tp->U.V13.tsubconst = pconst();
  linkup(tp, tp->U.V13.tsubconst);
 } else
  tp->U.V13.tsubconst = (struct S64 *)NIL;
 if (currsym.st == stype) {
  tp->U.V13.tsubtype = ptype();
  linkup(tp, tp->U.V13.tsubtype);
 } else
  tp->U.V13.tsubtype = (struct S64 *)NIL;
 if (currsym.st == svar) {
  tp->U.V13.tsubvar = pvar();
  linkup(tp, tp->U.V13.tsubvar);
 } else
  tp->U.V13.tsubvar = (struct S64 *)NIL;
 tp->U.V13.tsubsub = (struct S64 *)NIL;
 tq = (struct S64 *)NIL;
 while ((currsym.st == sproc) || (currsym.st == sfunc)) {
  if (tq == (struct S64 *)NIL) {
   tq = psubs();
   tp->U.V13.tsubsub = tq;
  } else {
   tq->tnext = psubs();
   tq = tq->tnext;
  }
 }
 linkup(tp, tp->U.V13.tsubsub);
 checksymbol(*((symset *)SETALIGN(Conset[7])));
 if (currsym.st == sbegin) {
  tp->U.V13.tsubstmt = pbegin(false);
  linkup(tp, tp->U.V13.tsubstmt);
 }
 statlvl = statlvl - 1;
}

treeptr pprogram();

 treeptr
pprmlist()
{
 register treeptr R133;
 treeptr tp, tq;
 idptr din, dut;

 tp = (struct S64 *)NIL;
 din = deftab.A[(int)(dinput)]->U.V14.tidl->U.V43.tsym->U.V6.lid;
 dut = deftab.A[(int)(doutput)]->U.V14.tidl->U.V43.tsym->U.V6.lid;
 while ((currsym.U.V1.vid == din) || (currsym.U.V1.vid == dut)) {
  if (currsym.U.V1.vid == din)
   defnams.A[(int)(dinput)]->U.V6.lused = true;
  else
   defnams.A[(int)(doutput)]->U.V6.lused = true;
  nextsymbol(*((symset *)SETALIGN(Conset[8])));
  if (currsym.st == srpar)
   goto L999;
  nextsymbol(*((symset *)SETALIGN(Conset[9])));
 }
 tq = newid(currsym.U.V1.vid);
 tq->U.V43.tsym->lt = lpointer;
 tp = tq;
 nextsymbol(*((symset *)SETALIGN(Conset[10])));
 while (currsym.st == scomma) {
  nextsymbol(*((symset *)SETALIGN(Conset[11])));
  if (currsym.U.V1.vid == din)
   defnams.A[(int)(dinput)]->U.V6.lused = true;
  else
   if (currsym.U.V1.vid == dut)
    defnams.A[(int)(doutput)]->U.V6.lused = true;
   else {
    tq->tnext = newid(currsym.U.V1.vid);
    tq = tq->tnext;
    tq->U.V43.tsym->lt = lpointer;
   }
  nextsymbol(*((symset *)SETALIGN(Conset[12])));
 }
L999:
 R133 = tp;
 return R133;
}

 treeptr
pprogram()
{
 register treeptr R132;
 treeptr tp;

 enterscope((declptr)NIL);
 tp = mknode(npgm);
 nextsymbol(*((symset *)SETALIGN(Conset[13])));
 tp->U.V13.tstat = statlvl;
 tp->U.V13.tsubid = mknode(nid);
 tp->U.V13.tsubid->tup = tp;
 tp->U.V13.tsubid->U.V43.tsym = mksym(lidentifier);
 tp->U.V13.tsubid->U.V43.tsym->U.V6.lid = currsym.U.V1.vid;
 tp->U.V13.tsubid->U.V43.tsym->lsymdecl = tp->U.V13.tsubid;
 linkup(tp, tp->U.V13.tsubid);
 nextsymbol(*((symset *)SETALIGN(Conset[14])));
 if (currsym.st == slpar) {
  nextsymbol(*((symset *)SETALIGN(Conset[15])));
  tp->U.V13.tsubpar = pprmlist();
  linkup(tp, tp->U.V13.tsubpar);
  nextsymbol(*((symset *)SETALIGN(Conset[16])));
 } else
  tp->U.V13.tsubpar = (struct S64 *)NIL;
 nextsymbol(*((symset *)SETALIGN(Conset[17])));
 pbody(tp);
 checksymbol(*((symset *)SETALIGN(Conset[18])));
 nextsymbol(*((symset *)SETALIGN(Conset[19])));
 tp->U.V13.tscope = currscope();
 leavescope();
 R132 = tp;
 return R132;
}

 treeptr
pmodule()
{
 register treeptr R134;
 treeptr tp;

 enterscope((declptr)NIL);
 tp = mknode(npgm);
 tp->U.V13.tstat = statlvl;
 tp->U.V13.tsubid = (struct S64 *)NIL;
 tp->U.V13.tsubpar = (struct S64 *)NIL;
 pbody(tp);
 checksymbol(*((symset *)SETALIGN(Conset[20])));
 if (currsym.st == ssemic)
  nextsymbol(*((symset *)SETALIGN(Conset[21])));
 tp->U.V13.tscope = currscope();
 leavescope();
 R134 = tp;
 return R134;
}

 treeptr
plabel()
{
 register treeptr R135;
 treeptr tp, tq;

 tq = (struct S64 *)NIL;
 do {
  nextsymbol(*((symset *)SETALIGN(Conset[22])));
  if (tq == (struct S64 *)NIL) {
   tq = newlbl();
   tp = tq;
  } else {
   tq->tnext = newlbl();
   tq = tq->tnext;
  }
  nextsymbol(*((symset *)SETALIGN(Conset[23])));
 } while (!(currsym.st == ssemic));
 nextsymbol(*((symset *)SETALIGN(Conset[24])));
 R135 = tp;
 return R135;
}

 treeptr
pidlist(l)
 ltypes l;
{
 register treeptr R136;
 treeptr tp, tq;

 tq = newid(currsym.U.V1.vid);
 tq->U.V43.tsym->lt = l;
 tp = tq;
 nextsymbol(*((symset *)SETALIGN(Conset[25])));
 while (currsym.st == scomma) {
  nextsymbol(*((symset *)SETALIGN(Conset[26])));
  tq->tnext = newid(currsym.U.V1.vid);
  tq = tq->tnext;
  tq->U.V43.tsym->lt = l;
  nextsymbol(*((symset *)SETALIGN(Conset[27])));
 }
 R136 = tp;
 return R136;
}

 treeptr
pconst()
{
 register treeptr R137;
 treeptr tp, tq;

 tq = (struct S64 *)NIL;
 nextsymbol(*((symset *)SETALIGN(Conset[28])));
 do {
  if (tq == (struct S64 *)NIL) {
   tq = mknode(nconst);
   tq->U.V14.tattr = anone;
   tp = tq;
  } else {
   tq->tnext = mknode(nconst);
   tq = tq->tnext;
   tq->U.V14.tattr = anone;
  }
  tq->U.V14.tidl = pidlist(lidentifier);
  checksymbol(*((symset *)SETALIGN(Conset[29])));
  nextsymbol(*((symset *)SETALIGN(Conset[30])));
  tq->U.V14.tbind = pconstant(true);
  nextsymbol(*((symset *)SETALIGN(Conset[31])));
  nextsymbol(*((symset *)SETALIGN(Conset[32])));
 } while (!(currsym.st != sid));
 R137 = tp;
 return R137;
}

 treeptr
pconstant(realok)
 boolean realok;
{
 register treeptr R138;
 treeptr tp, tq;
 boolean neg;

 neg = (boolean)(currsym.st == sminus);
 if (Member((unsigned)(currsym.st), Conset[33]))
  if (realok)
   nextsymbol(*((symset *)SETALIGN(Conset[34])));
  else
   nextsymbol(*((symset *)SETALIGN(Conset[35])));
 if (currsym.st == sid)
  tp = oldid(currsym.U.V1.vid, lidentifier);
 else
  tp = mklit();
 if (neg) {
  tq = mknode(numinus);
  tq->U.V42.texps = tp;
  tp = tq;
 }
 R138 = tp;
 return R138;
}

 treeptr
precord(cs, dp)
 symtyp cs;
 declptr dp;
{
 register treeptr R139;
 treeptr tp, tq, tl, tv;
 lexsym tsym;

 tp = mknode(nrecord);
 tp->U.V21.tflist = (struct S64 *)NIL;
 tp->U.V21.tvlist = (struct S64 *)NIL;
 tp->U.V21.tuid = (struct S62 *)NIL;
 tp->U.V21.trscope = (struct S63 *)NIL;
 if (cs == send) {
  enterscope(dp);
  dp = currscope();
 }
 nextsymbol(*((symset *)SETALIGN((Tmpset = Newset(), (void)Insmem((unsigned)(sid), Tmpset),
  (void)Insmem((unsigned)(scase), Tmpset),
  (void)Insmem((unsigned)(cs), Tmpset), Tmpset))));
 tq = (struct S64 *)NIL;
 while (currsym.st == sid) {
  if (tq == (struct S64 *)NIL) {
   tq = mknode(nfield);
   tq->U.V14.tattr = anone;
   tp->U.V21.tflist = tq;
  } else {
   tq->tnext = mknode(nfield);
   tq = tq->tnext;
   tq->U.V14.tattr = anone;
  }
  tq->U.V14.tidl = pidlist(lfield);
  checksymbol(*((symset *)SETALIGN(Conset[36])));
  leavescope();
  tq->U.V14.tbind = ptypedef();
  enterscope(dp);
  if (currsym.st == ssemic)
   nextsymbol(*((symset *)SETALIGN((Tmpset = Newset(), (void)Insmem((unsigned)(sid), Tmpset),
    (void)Insmem((unsigned)(scase), Tmpset),
    (void)Insmem((unsigned)(cs), Tmpset), Tmpset))));
 }
 if (currsym.st == scase) {
  nextsymbol(*((symset *)SETALIGN(Conset[37])));
  tsym = currsym;
  nextsymbol(*((symset *)SETALIGN(Conset[38])));
  if (currsym.st == scolon) {
   tv = newid(tsym.U.V1.vid);
   if (tq == (struct S64 *)NIL) {
    tq = mknode(nfield);
    tp->U.V21.tflist = tq;
   } else {
    tq->tnext = mknode(nfield);
    tq = tq->tnext;
   }
   tq->U.V14.tidl = tv;
   tv->U.V43.tsym->lt = lfield;
   nextsymbol(*((symset *)SETALIGN(Conset[39])));
   leavescope();
   tq->U.V14.tbind = oldid(currsym.U.V1.vid, lidentifier);
   enterscope(dp);
   nextsymbol(*((symset *)SETALIGN(Conset[40])));
  }
  tq = (struct S64 *)NIL;
  do {
   tv = (struct S64 *)NIL;
   do {
    nextsymbol(*((symset *)SETALIGN((Tmpset = Newset(), (void)Insmem((unsigned)(sid), Tmpset),
     (void)Insmem((unsigned)(sinteger), Tmpset),
     (void)Insmem((unsigned)(schar), Tmpset),
     (void)Insmem((unsigned)(splus), Tmpset),
     (void)Insmem((unsigned)(sminus), Tmpset),
     (void)Insmem((unsigned)(cs), Tmpset), Tmpset))));
    if (currsym.st == cs)
     goto L999;
    if (tv == (struct S64 *)NIL) {
     tv = pconstant(false);
     tl = tv;
    } else {
     tv->tnext = pconstant(false);
     tv = tv->tnext;
    }
    nextsymbol(*((symset *)SETALIGN(Conset[41])));
   } while (!(currsym.st == scolon));
   nextsymbol(*((symset *)SETALIGN(Conset[42])));
   if (tq == (struct S64 *)NIL) {
    tq = mknode(nvariant);
    tp->U.V21.tvlist = tq;
   } else {
    tq->tnext = mknode(nvariant);
    tq = tq->tnext;
   }
   tq->U.V20.tselct = tl;
   tq->U.V20.tvrnt = precord(srpar, dp);
  } while (!(currsym.st == cs));
 }
L999:
 if (cs == send) {
  tp->U.V21.trscope = dp;
  leavescope();
 }
 nextsymbol(*((symset *)SETALIGN(Conset[43])));
 R139 = tp;
 return R139;
}

 treeptr
ptypedef()
{
 register treeptr R140;
 treeptr tp, tq;
 symtyp st;
 symset ss;

 nextsymbol(*((symset *)SETALIGN(Conset[44])));
 if (currsym.st == spacked)
  nextsymbol(*((symset *)SETALIGN(Conset[45])));
 Setncpy(ss.S, Conset[46], sizeof(ss.S));
 switch ((int)(currsym.st)) {
   case splus:  case sminus:  case schar:  case sinteger:
   case sid:
  st = currsym.st;
  tp = pconstant(false);
  if (st == sid)
   nextsymbol(*((symset *)SETALIGN(Union(Conset[47], ss.S))));
  else
   nextsymbol(*((symset *)SETALIGN(Conset[48])));
  Claimset();
  if (currsym.st == sdotdot) {
   nextsymbol(*((symset *)SETALIGN(Conset[49])));
   tq = mknode(nsubrange);
   tq->U.V19.tlo = tp;
   tq->U.V19.thi = pconstant(false);
   tp = tq;
   nextsymbol(ss);
  }
  break ;
   case slpar:
  tp = mknode(nscalar);
  nextsymbol(*((symset *)SETALIGN(Conset[50])));
  tp->U.V17.tscalid = pidlist(lidentifier);
  checksymbol(*((symset *)SETALIGN(Conset[51])));
  nextsymbol(ss);
  break ;
   case sarrow:
  tp = mknode(nptr);
  nextsymbol(*((symset *)SETALIGN(Conset[52])));
  tp->U.V16.tptrid = oldid(currsym.U.V1.vid, lpointer);
  tp->U.V16.tptrflag = false;
  nextsymbol(*((symset *)SETALIGN(Conset[53])));
  break ;
   case sarray:
  nextsymbol(*((symset *)SETALIGN(Conset[54])));
  tp = mknode(narray);
  tp->U.V23.taindx = ptypedef();
  tq = tp;
  while (currsym.st == scomma) {
   tq->U.V23.taelem = mknode(narray);
   tq = tq->U.V23.taelem;
   tq->U.V23.taindx = ptypedef();
  }
  checksymbol(*((symset *)SETALIGN(Conset[55])));
  nextsymbol(*((symset *)SETALIGN(Conset[56])));
  tq->U.V23.taelem = ptypedef();
  break ;
   case srecord:
  tp = precord(send, (declptr)NIL);
  break ;
   case sfile:  case sset:
  if (currsym.st == sfile)
   tp = mknode(nfileof);
  else {
   tp = mknode(nsetof);
   usesets = true;
  }
  nextsymbol(*((symset *)SETALIGN(Conset[57])));
  tp->U.V18.tof = ptypedef();
  break ;
   default:
  Caseerror(Line);
 }
 R140 = tp;
 return R140;
}

 treeptr
ptype()
{
 register treeptr R141;
 treeptr tp, tq;

 tq = (struct S64 *)NIL;
 nextsymbol(*((symset *)SETALIGN(Conset[58])));
 do {
  if (tq == (struct S64 *)NIL) {
   tq = mknode(ntype);
   tq->U.V14.tattr = anone;
   tp = tq;
  } else {
   tq->tnext = mknode(ntype);
   tq = tq->tnext;
   tq->U.V14.tattr = anone;
  }
  tq->U.V14.tidl = pidlist(lidentifier);
  checksymbol(*((symset *)SETALIGN(Conset[59])));
  tq->U.V14.tbind = ptypedef();
  nextsymbol(*((symset *)SETALIGN(Conset[60])));
 } while (!(currsym.st != sid));
 R141 = tp;
 return R141;
}

 treeptr
pvar()
{
 register treeptr R142;
 treeptr ti, tp, tq;

 tq = (struct S64 *)NIL;
 nextsymbol(*((symset *)SETALIGN(Conset[61])));
 do {
  if (tq == (struct S64 *)NIL) {
   tq = mknode(nvar);
   tq->U.V14.tattr = anone;
   tp = tq;
  } else {
   tq->tnext = mknode(nvar);
   tq = tq->tnext;
   tq->U.V14.tattr = anone;
  }
  ti = newid(currsym.U.V1.vid);
  tq->U.V14.tidl = ti;
  nextsymbol(*((symset *)SETALIGN(Conset[62])));
  while (currsym.st == scomma) {
   nextsymbol(*((symset *)SETALIGN(Conset[63])));
   ti->tnext = newid(currsym.U.V1.vid);
   ti = ti->tnext;
   nextsymbol(*((symset *)SETALIGN(Conset[64])));
  }
  tq->U.V14.tbind = ptypedef();
  nextsymbol(*((symset *)SETALIGN(Conset[65])));
 } while (!(currsym.st != sid));
 R142 = tp;
 return R142;
}

 treeptr
psubs()
{
 register treeptr R143;
 treeptr tp, tv, tq;
 boolean func;
 symtyp colsem;

 func = (boolean)(currsym.st == sfunc);
 if (func)
  colsem = scolon;
 else
  colsem = ssemic;
 nextsymbol(*((symset *)SETALIGN(Conset[66])));
 tq = newid(currsym.U.V1.vid);
 if (tq->tup == (struct S64 *)NIL) {
  enterscope((declptr)NIL);
  if (func)
   tp = mknode(nfunc);
  else
   tp = mknode(nproc);
  tp->U.V13.tstat = statlvl;
  tp->U.V13.tsubid = tq;
  linkup(tp, tq);
  nextsymbol(*((symset *)SETALIGN((Tmpset = Newset(), (void)Insmem((unsigned)(slpar), Tmpset),
   (void)Insmem((unsigned)(colsem), Tmpset), Tmpset))));
  if (currsym.st == slpar) {
   tp->U.V13.tsubpar = psubpar();
   linkup(tp, tp->U.V13.tsubpar);
   nextsymbol(*((symset *)SETALIGN((Tmpset = Newset(), (void)Insmem((unsigned)(colsem), Tmpset), Tmpset))));
  } else
   tp->U.V13.tsubpar = (struct S64 *)NIL;
  if (func) {
   nextsymbol(*((symset *)SETALIGN(Conset[67])));
   tp->U.V13.tfuntyp = oldid(currsym.U.V1.vid, lidentifier);
   nextsymbol(*((symset *)SETALIGN(Conset[68])));
  } else
   tp->U.V13.tfuntyp = mknode(nempty);
  linkup(tp, tp->U.V13.tfuntyp);
  nextsymbol(*((symset *)SETALIGN(Conset[69])));
 } else {
  enterscope(tq->tup->U.V13.tscope);
  if (func)
   tp = mknode(nfunc);
  else
   tp = mknode(nproc);
  tp->U.V13.tfuntyp = tq->tup->U.V13.tfuntyp;
  tv = tq->tup->U.V13.tsubpar;
  tp->U.V13.tsubpar = tv;
  while (tv != (struct S64 *)NIL) {
   tv->tup = tp;
   tv = tv->tnext;
  }
  tp->U.V13.tsubid = tq;
  tq->tup = tp;
  nextsymbol(*((symset *)SETALIGN(Conset[70])));
  nextsymbol(*((symset *)SETALIGN(Conset[71])));
 }
 if (Member((unsigned)(currsym.st), Conset[72])) {
  tp->U.V13.tsubid->U.V43.tsym->lt = lforward;
  nextsymbol(*((symset *)SETALIGN(Conset[73])));
  tp->U.V13.tsublab = (struct S64 *)NIL;
  tp->U.V13.tsubconst = (struct S64 *)NIL;
  tp->U.V13.tsubtype = (struct S64 *)NIL;
  tp->U.V13.tsubvar = (struct S64 *)NIL;
  tp->U.V13.tsubsub = (struct S64 *)NIL;
  tp->U.V13.tsubstmt = (struct S64 *)NIL;
 } else
  pbody(tp);
 nextsymbol(*((symset *)SETALIGN(Conset[74])));
 tp->U.V13.tscope = currscope();
 leavescope();
 R143 = tp;
 return R143;
}

 treeptr
pconfsub()
{
 register treeptr R144;
 treeptr tp;

 tp = mknode(nsubrange);
 nextsymbol(*((symset *)SETALIGN(Conset[75])));
 tp->U.V19.tlo = newid(currsym.U.V1.vid);
 nextsymbol(*((symset *)SETALIGN(Conset[76])));
 nextsymbol(*((symset *)SETALIGN(Conset[77])));
 tp->U.V19.thi = newid(currsym.U.V1.vid);
 nextsymbol(*((symset *)SETALIGN(Conset[78])));
 R144 = tp;
 return R144;
}

 treeptr
pconform()
{
 register treeptr R145;
 treeptr tp, tq;

 nextsymbol(*((symset *)SETALIGN(Conset[79])));
 tp = mknode(nconfarr);
 tp->U.V22.tcuid = mkvariable('S');
 tp->U.V22.tcindx = pconfsub();
 nextsymbol(*((symset *)SETALIGN(Conset[80])));
 tp->U.V22.tindtyp = oldid(currsym.U.V1.vid, lidentifier);
 nextsymbol(*((symset *)SETALIGN(Conset[81])));
 tq = tp;
 while (currsym.st == ssemic) {
  error(econfconf);
  tq->U.V22.tcelem = mknode(nconfarr);
  tq = tq->U.V22.tcelem;
  tq->U.V22.tcindx = pconfsub();
  nextsymbol(*((symset *)SETALIGN(Conset[82])));
  tq->U.V22.tindtyp = oldid(currsym.U.V1.vid, lidentifier);
  nextsymbol(*((symset *)SETALIGN(Conset[83])));
 }
 nextsymbol(*((symset *)SETALIGN(Conset[84])));
 nextsymbol(*((symset *)SETALIGN(Conset[85])));
 switch ((int)(currsym.st)) {
   case sid:
  tq->U.V22.tcelem = oldid(currsym.U.V1.vid, lidentifier);
  break ;
   case sarray:
  error(econfconf);
  tq->U.V22.tcelem = pconform();
  break ;
   default:
  Caseerror(Line);
 }
 R145 = tp;
 return R145;
}

 treeptr
psubpar()
{
 register treeptr R146;
 treeptr tp, tq;
 treetyp nt;

 tq = (struct S64 *)NIL;
 do {
  nextsymbol(*((symset *)SETALIGN(Conset[86])));
  switch ((int)(currsym.st)) {
    case sid:
   nt = nvalpar;
   break ;
    case svar:
   nt = nvarpar;
   break ;
    case sfunc:
   nt = nparfunc;
   break ;
    case sproc:
   nt = nparproc;
   break ;
    default:
   Caseerror(Line);
  }
  if (nt != nvalpar)
   nextsymbol(*((symset *)SETALIGN(Conset[87])));
  if (tq == (struct S64 *)NIL) {
   tq = mknode(nt);
   tp = tq;
  } else {
   tq->tnext = mknode(nt);
   tq = tq->tnext;
  }
  switch ((int)(nt)) {
    case nvarpar:  case nvalpar:
   tq->U.V14.tidl = pidlist(lidentifier);
   tq->U.V14.tattr = anone;
   checksymbol(*((symset *)SETALIGN(Conset[88])));
   if (nt == nvalpar)
    nextsymbol(*((symset *)SETALIGN(Conset[89])));
   else
    nextsymbol(*((symset *)SETALIGN(Conset[90])));
   switch ((int)(currsym.st)) {
     case sid:
    tq->U.V14.tbind = oldid(currsym.U.V1.vid, lidentifier);
    break ;
     case sarray:
    tq->U.V14.tbind = pconform();
    break ;
     default:
    Caseerror(Line);
   }
   nextsymbol(*((symset *)SETALIGN(Conset[91])));
   break ;
    case nparproc:
   tq->U.V15.tparid = newid(currsym.U.V1.vid);
   nextsymbol(*((symset *)SETALIGN(Conset[92])));
   if (currsym.st == slpar) {
    enterscope((declptr)NIL);
    tq->U.V15.tparparm = psubpar();
    nextsymbol(*((symset *)SETALIGN(Conset[93])));
    leavescope();
   } else
    tq->U.V15.tparparm = (struct S64 *)NIL;
   tq->U.V15.tpartyp = (struct S64 *)NIL;
   break ;
    case nparfunc:
   tq->U.V15.tparid = newid(currsym.U.V1.vid);
   nextsymbol(*((symset *)SETALIGN(Conset[94])));
   if (currsym.st == slpar) {
    enterscope((declptr)NIL);
    tq->U.V15.tparparm = psubpar();
    nextsymbol(*((symset *)SETALIGN(Conset[95])));
    leavescope();
   } else
    tq->U.V15.tparparm = (struct S64 *)NIL;
   nextsymbol(*((symset *)SETALIGN(Conset[96])));
   tq->U.V15.tpartyp = oldid(currsym.U.V1.vid, lidentifier);
   nextsymbol(*((symset *)SETALIGN(Conset[97])));
   break ;
    default:
   Caseerror(Line);
  }
 } while (!(currsym.st == srpar));
 R146 = tp;
 return R146;
}

 treeptr
plabstmt()
{
 register treeptr R147;
 treeptr tp;

 nextsymbol(*((symset *)SETALIGN(Conset[98])));
 if (currsym.st == sinteger) {
  tp = mknode(nlabstmt);
  tp->U.V25.tlabno = oldlbl(true);
  nextsymbol(*((symset *)SETALIGN(Conset[99])));
  nextsymbol(*((symset *)SETALIGN(Conset[100])));
  tp->U.V25.tstmt = pstmt();
 } else
  tp = pstmt();
 R147 = tp;
 return R147;
}

 treeptr
pstmt()
{
 register treeptr R148;
 treeptr tp;

 switch ((int)(currsym.st)) {
   case sid:
  tp = psimple();
  break ;
   case sif:
  tp = pif();
  break ;
   case swhile:
  tp = pwhile();
  break ;
   case srepeat:
  tp = prepeat();
  break ;
   case sfor:
  tp = pfor();
  break ;
   case scase:
  tp = pcase();
  break ;
   case swith:
  tp = pwith();
  break ;
   case sbegin:
  tp = pbegin(true);
  break ;
   case sgoto:
  tp = pgoto();
  break ;
   case send:  case selse:  case suntil:  case ssemic:
  tp = mknode(nempty);
  break ;
   default:
  Caseerror(Line);
 }
 R148 = tp;
 return R148;
}

 treeptr
psimple()
{
 register treeptr R149;
 treeptr tq, tp;

 tp = pvariable(oldid(currsym.U.V1.vid, lidentifier));
 if (currsym.st == sassign) {
  tq = mknode(nassign);
  tq->U.V27.tlhs = tp;
  tq->U.V27.trhs = pexpr((treeptr)NIL);
  tp = tq;
 }
 R149 = tp;
 return R149;
}

 treeptr
pvariable(varptr)
 treeptr varptr;
{
 register treeptr R150;
 treeptr tp, tq, tv;

 nextsymbol(*((symset *)SETALIGN(Conset[101])));
 if (Member((unsigned)(currsym.st), Conset[102])) {
  switch ((int)(currsym.st)) {
    case slpar:
   tp = mknode(ncall);
   tp->U.V30.tcall = varptr;
   tq = (struct S64 *)NIL;
   do {
    if (tq == (struct S64 *)NIL) {
     tq = pexpr((treeptr)NIL);
     tp->U.V30.taparm = tq;
    } else {
     tq->tnext = pexpr((treeptr)NIL);
     tq = tq->tnext;
    }
   } while (!(currsym.st == srpar));
   break ;
    case slbrack:
   tq = varptr;
   do {
    tp = mknode(nindex);
    tp->U.V39.tvariable = tq;
    tp->U.V39.toffset = pexpr((treeptr)NIL);
    tv = typeof(tq);
    if (!((tv->tt == narray) || (tv->tt == nconfarr) || (tv == typnods.A[(int)(tstring)])))
     error(eindex);
    tq = tp;
   } while (!(currsym.st == srbrack));
   break ;
    case sdot:
   tp = mknode(nselect);
   tp->U.V40.trecord = varptr;
   nextsymbol(*((symset *)SETALIGN(Conset[103])));
   tq = typeof(varptr);
   if (tq->tt != nrecord)
    error(enotrecord);
   enterscope(tq->U.V21.trscope);
   tp->U.V40.tfield = oldid(currsym.U.V1.vid, lfield);
   leavescope();
   break ;
    case sarrow:
   tq = typeof(varptr);
   if (!((tq->tt == nptr) || (tq->tt == nfileof) || (tq == typnods.A[(int)(ttext)])))
    error(enotpointer);
   tp = mknode(nderef);
   tp->U.V42.texps = varptr;
   break ;
    default:
   Caseerror(Line);
  }
  tp = pvariable(tp);
 } else {
  tp = varptr;
  if (tp->tt == nid) {
   tq = idup(tp);
   if (tq != (struct S64 *)NIL)
    if (Member((unsigned)(tq->tt), Conset[104])) {
     tp = mknode(ncall);
     tp->U.V30.tcall = varptr;
     tp->U.V30.taparm = (struct S64 *)NIL;
    }
  }
 }
 R150 = tp;
 return R150;
}

treeptr pexpr();

 treeptr
padjust(tu, tr)
 treeptr tu, tr;
{
 register treeptr R152;

 if (pprio.A[(int)(tu->tt) - (int)(nassign)] >= pprio.A[(int)(tr->tt) - (int)(nassign)]) {
  if (Member((unsigned)(tr->tt), Conset[105]))
   tr->U.V42.texps = padjust(tu, tr->U.V42.texps);
  else
   tr->U.V41.texpl = padjust(tu, tr->U.V41.texpl);
  R152 = tr;
 } else {
  if (Member((unsigned)(tu->tt), Conset[106]))
   tu->U.V42.texps = tr;
  else
   tu->U.V41.texpr = tr;
  R152 = tu;
 }
 return R152;
}

 treeptr
pexpr(tnp)
 treeptr tnp;
{
 register treeptr R151;
 treeptr tp, tq;
 treetyp nt;
 boolean next;

 nextsymbol(*((symset *)SETALIGN(Conset[107])));
 next = true;
 switch ((int)(currsym.st)) {
   case splus:
  tp = mknode(nuplus);
  tp->U.V42.tuplusop = true;
  tp->U.V42.texps = (struct S64 *)NIL;
  tp = pexpr(tp);
  next = false;
  break ;
   case sminus:
  tp = mknode(numinus);
  tp->U.V42.texps = (struct S64 *)NIL;
  tp = pexpr(tp);
  next = false;
  break ;
   case snot:
  tp = mknode(nnot);
  tp->U.V42.texps = (struct S64 *)NIL;
  tp = pexpr(tp);
  next = false;
  break ;
   case schar:  case sinteger:  case sreal:  case sstring:
  tp = mklit();
  break ;
   case snil:
  usenilp = true;
  tp = mknode(nnil);
  break ;
   case sid:
  tp = pvariable(oldid(currsym.U.V1.vid, lidentifier));
  next = false;
  break ;
   case slpar:
  tp = mknode(nuplus);
  tp->U.V42.texps = pexpr((treeptr)NIL);
  break ;
   case slbrack:
  usesets = true;
  tp = mknode(nset);
  tp->U.V42.texps = (struct S64 *)NIL;
  tq = (struct S64 *)NIL;
  do {
   if (tq == (struct S64 *)NIL) {
    tq = pexpr((treeptr)NIL);
    tp->U.V42.texps = tq;
   } else {
    tq->tnext = pexpr((treeptr)NIL);
    tq = tq->tnext;
   }
  } while (!(currsym.st == srbrack));
  break ;
   case srbrack:
  tp = mknode(nempty);
  next = false;
  break ;
   default:
  Caseerror(Line);
 }
 if (next)
  nextsymbol(*((symset *)SETALIGN(Conset[108])));
 switch ((int)(currsym.st)) {
   case sdotdot:
  nt = nrange;
  break ;
   case splus:
  nt = nplus;
  break ;
   case sminus:
  nt = nminus;
  break ;
   case smul:
  nt = nmul;
  break ;
   case sdiv:
  nt = ndiv;
  break ;
   case smod:
  nt = nmod;
  break ;
   case squot:
  defnams.A[(int)(dreal)]->U.V6.lused = true;
  nt = nquot;
  break ;
   case sand:
  nt = nand;
  break ;
   case sor:
  nt = nor;
  break ;
   case sinn:
  nt = nin;
  usesets = true;
  break ;
   case sle:
  nt = nle;
  break ;
   case slt:
  nt = nlt;
  break ;
   case seq:
  nt = neq;
  break ;
   case sge:
  nt = nge;
  break ;
   case sgt:
  nt = ngt;
  break ;
   case sne:
  nt = nne;
  break ;
   case scolon:
  nt = nformat;
  break ;
   case sid:  case schar:  case sinteger:  case sreal:
   case sstring:  case snil:  case ssemic:  case scomma:
   case slpar:  case slbrack:  case srpar:  case srbrack:
   case send:  case suntil:  case sthen:  case selse:
   case sdo:  case sdownto:  case sto:  case sof:
  nt = nnil;
  break ;
   default:
  Caseerror(Line);
 }
 if (Member((unsigned)(nt), Conset[109]))
  defnams.A[(int)(dboolean)]->U.V6.lused = true;
 if (nt != nnil) {
  tq = mknode(nt);
  tq->U.V41.texpl = tp;
  tq->U.V41.texpr = (struct S64 *)NIL;
  tp = pexpr(tq);
 }
 if (tnp != (struct S64 *)NIL)
  tp = padjust(tnp, tp);
 R151 = tp;
 return R151;
}

 treeptr
pcase()
{
 register treeptr R153;
 treeptr tp, tq, tv;

 tp = mknode(ncase);
 tp->U.V35.tcasxp = pexpr((treeptr)NIL);
 checksymbol(*((symset *)SETALIGN(Conset[110])));
 tq = (struct S64 *)NIL;
 do {
  if (tq == (struct S64 *)NIL) {
   tq = mknode(nchoise);
   tp->U.V35.tcaslst = tq;
  } else {
   tq->tnext = mknode(nchoise);
   tq = tq->tnext;
  }
  tq->U.V36.tchocon = (struct S64 *)NIL;
  tq->U.V36.tchostmt = (struct S64 *)NIL;
  tv = (struct S64 *)NIL;
  do {
   nextsymbol(*((symset *)SETALIGN(Conset[111])));
   if (Member((unsigned)(currsym.st), Conset[112]))
    goto L999;
   if (tv == (struct S64 *)NIL) {
    tv = pconstant(false);
    tq->U.V36.tchocon = tv;
   } else {
    tv->tnext = pconstant(false);
    tv = tv->tnext;
   }
   nextsymbol(*((symset *)SETALIGN(Conset[113])));
  } while (!(currsym.st == scolon));
  tq->U.V36.tchostmt = plabstmt();
 } while (!(currsym.st == send));
L999:
 if (currsym.st == sother) {
  nextsymbol(*((symset *)SETALIGN(Conset[114])));
  if (currsym.st == scolon)
   nextsymbol(*((symset *)SETALIGN(Conset[115])));
  tp->U.V35.tcasother = pstmt();
 } else {
  tp->U.V35.tcasother = (struct S64 *)NIL;
  usecase = true;
 }
 nextsymbol(*((symset *)SETALIGN(Conset[116])));
 R153 = tp;
 return R153;
}

 treeptr
pif()
{
 register treeptr R154;
 treeptr tp;

 tp = mknode(nif);
 tp->U.V31.tifxp = pexpr((treeptr)NIL);
 checksymbol(*((symset *)SETALIGN(Conset[117])));
 tp->U.V31.tthen = plabstmt();
 if (currsym.st == selse)
  tp->U.V31.telse = plabstmt();
 else
  tp->U.V31.telse = (struct S64 *)NIL;
 R154 = tp;
 return R154;
}

 treeptr
pwhile()
{
 register treeptr R155;
 treeptr tp;

 tp = mknode(nwhile);
 tp->U.V32.twhixp = pexpr((treeptr)NIL);
 checksymbol(*((symset *)SETALIGN(Conset[118])));
 tp->U.V32.twhistmt = plabstmt();
 R155 = tp;
 return R155;
}

 treeptr
prepeat()
{
 register treeptr R156;
 treeptr tp, tq;

 tp = mknode(nrepeat);
 tq = (struct S64 *)NIL;
 do {
  if (tq == (struct S64 *)NIL) {
   tq = plabstmt();
   tp->U.V33.treptstmt = tq;
  } else {
   tq->tnext = plabstmt();
   tq = tq->tnext;
  }
  checksymbol(*((symset *)SETALIGN(Conset[119])));
 } while (!(currsym.st == suntil));
 tp->U.V33.treptxp = pexpr((treeptr)NIL);
 R156 = tp;
 return R156;
}

 treeptr
pfor()
{
 register treeptr R157;
 treeptr tp;

 tp = mknode(nfor);
 nextsymbol(*((symset *)SETALIGN(Conset[120])));
 tp->U.V34.tforid = oldid(currsym.U.V1.vid, lidentifier);
 nextsymbol(*((symset *)SETALIGN(Conset[121])));
 tp->U.V34.tfrom = pexpr((treeptr)NIL);
 checksymbol(*((symset *)SETALIGN(Conset[122])));
 tp->U.V34.tincr = (boolean)(currsym.st == sto);
 tp->U.V34.tto = pexpr((treeptr)NIL);
 checksymbol(*((symset *)SETALIGN(Conset[123])));
 tp->U.V34.tforstmt = plabstmt();
 R157 = tp;
 return R157;
}

 treeptr
pwith()
{
 register treeptr R158;
 treeptr tp, tq;

 tp = mknode(nwith);
 tq = (struct S64 *)NIL;
 do {
  if (tq == (struct S64 *)NIL) {
   tq = mknode(nwithvar);
   tp->U.V37.twithvar = tq;
  } else {
   tq->tnext = mknode(nwithvar);
   tq = tq->tnext;
  }
  enterscope((declptr)NIL);
  tq->U.V38.tenv = currscope();
  tq->U.V38.texpw = pexpr((treeptr)NIL);
  if (typeof(tq->U.V38.texpw)->tt != nrecord)
   error(enotrecord);
  scopeup(tq->U.V38.texpw);
  checksymbol(*((symset *)SETALIGN(Conset[124])));
 } while (!(currsym.st == sdo));
 tp->U.V37.twithstmt = plabstmt();
 tq = tp->U.V37.twithvar;
 while (tq != (struct S64 *)NIL) {
  leavescope();
  tq = tq->tnext;
 }
 R158 = tp;
 return R158;
}

 treeptr
pgoto()
{
 register treeptr R159;
 treeptr tp;

 nextsymbol(*((symset *)SETALIGN(Conset[125])));
 tp = mknode(ngoto);
 tp->U.V26.tlabel = oldlbl(false);
 nextsymbol(*((symset *)SETALIGN(Conset[126])));
 R159 = tp;
 return R159;
}

 treeptr
pbegin(retain)
 boolean retain;
{
 register treeptr R160;
 treeptr tp, tq;

 tq = (struct S64 *)NIL;
 do {
  if (tq == (struct S64 *)NIL) {
   tq = plabstmt();
   tp = tq;
  } else {
   tq->tnext = plabstmt();
   tq = tq->tnext;
  }
 } while (!(currsym.st == send));
 if (retain) {
  tq = mknode(nbegin);
  tq->U.V24.tbegin = tp;
  tp = tq;
 }
 nextsymbol(*((symset *)SETALIGN(Conset[127])));
 R160 = tp;
 return R160;
}

 void
parse()
{
 nextsymbol(*((symset *)SETALIGN(Conset[128])));
 if (currsym.st == spgm)
  top = pprogram();
 else
  top = pmodule();
}

integer cvalof();

void semcheck();

void checkexpr();

void checkstmt();

void checktype();

 void
semerror(tp, m)
 treeptr tp;
 errors m;
{
 if (tp != (struct S64 *)NIL) {
  lastline = tp->tline;
  lastcol = tp->tcol;
 }
 error(m);
}

 treeptr
declof(tp)
 treeptr tp;
{
 register treeptr R162;

 if ((tp == (struct S64 *)NIL) || (tp->tt != nid))
  R162 = (struct S64 *)NIL;
 else
  R162 = idup(tp);
 return R162;
}

 boolean
typeref(tp)
 treeptr tp;
{
 register boolean R163;
 treeptr tq;

 if (tp == (struct S64 *)NIL)
  R163 = false;
 else {
  switch ((int)(tp->tt)) {
    case nid:
   tq = idup(tp);
   if (tq == (struct S64 *)NIL)
    R163 = false;
   else
    R163 = (boolean)(tq->tt == ntype);
   break ;
    case npredef:  case nptr:  case nscalar:  case nrecord:
    case nconfarr:  case narray:  case nfileof:  case nsetof:
    case nsubrange:
   R163 = true;
   break ;
    default:
   R163 = false;
  }
 }
 return R163;
}

 treeptr
basetype(tp)
 treeptr tp;
{
 register treeptr R164;
 treeptr tq;

 tq = typeof(tp);
 while (tq->tt == nsubrange)
  tq = typeof(tq->U.V19.tlo);
 R164 = tq;
 return R164;
}

 boolean
ordinaltype(tp)
 treeptr tp;
{
 register boolean R165;
 treeptr tq;

 tq = basetype(tp);
 R165 = (boolean)((tq == typnods.A[(int)(tinteger)]) || (tq == typnods.A[(int)(tchar)]) || (tq == typnods.A[(int)(tboolean)]) || (tq->tt == nscalar));
 return R165;
}

 boolean
integertype(tp)
 treeptr tp;
{
 register boolean R166;

 R166 = (boolean)(basetype(tp) == typnods.A[(int)(tinteger)]);
 return R166;
}

 boolean
numerictype(tp)
 treeptr tp;
{
 register boolean R167;
 treeptr tq;

 tq = basetype(tp);
 R167 = (boolean)((tq == typnods.A[(int)(tinteger)]) || (tq == typnods.A[(int)(treal)]));
 return R167;
}

 boolean
filetype(tp)
 treeptr tp;
{
 register boolean R168;
 treeptr tq;

 tq = typeof(tp);
 R168 = (boolean)((tq == typnods.A[(int)(ttext)]) || (tq->tt == nfileof));
 return R168;
}

 boolean
chararray(tp)
 treeptr tp;
{
 register boolean R169;
 treeptr tq;

 tq = typeof(tp);
 if (tq->tt == narray)
  R169 = (boolean)(basetype(tq->U.V23.taelem) == typnods.A[(int)(tchar)]);
 else
  if (tq->tt == nconfarr)
   R169 = (boolean)(basetype(tq->U.V22.tcelem) == typnods.A[(int)(tchar)]);
  else
   R169 = false;
 return R169;
}

 boolean
sametype(tl, tr)
 treeptr tl, tr;
{
 register boolean R170;
 treeptr ta, tb;

 ta = typeof(tl);
 tb = typeof(tr);
 if (ta == tb)
  R170 = true;
 else {
  if (ta->tt == nsubrange)
   ta = basetype(ta);
  if (tb->tt == nsubrange)
   tb = basetype(tb);
  if (ta == tb)
   R170 = true;
  else
   if ((ta->tt == nsetof) && (tb->tt == nsetof))
    R170 = sametype(ta->U.V18.tof, tb->U.V18.tof);
   else
    R170 = false;
 }
 return R170;
}

 boolean
identicaltype(tl, tr)
 treeptr tl, tr;
{
 register boolean R171;
 treeptr ta, tb;

 ta = typeof(tl);
 tb = typeof(tr);
 if (ta == tb)
  R171 = true;
 else
  if ((ta->tt == nconfarr) && (tb->tt == narray))
   R171 = sametype(ta->U.V22.tcelem, tb->U.V23.taelem);
  else
   if ((ta->tt == narray) && (tb->tt == nconfarr))
    R171 = sametype(ta->U.V23.taelem, tb->U.V22.tcelem);
   else
    R171 = false;
 return R171;
}

 boolean
assignable(tl, tr)
 treeptr tl, tr;
{
 register boolean R172;
 treeptr ta, tb;

 ta = typeof(tl);
 tb = typeof(tr);
 if (sametype(ta, tb))
  R172 = true;
 else
  if ((basetype(ta) == typnods.A[(int)(treal)]) && (basetype(tb) == typnods.A[(int)(tinteger)]))
   R172 = true;
  else
   if ((ta->tt == nptr) && (tb == typnods.A[(int)(tnil)]))
    R172 = true;
   else
    if ((ta->tt == nsetof) && (tb == typnods.A[(int)(tset)]))
     R172 = true;
    else
     if (chararray(ta) && (tb == typnods.A[(int)(tstring)]))
      R172 = true;
     else
      R172 = false;
 return R172;
}

 boolean
constant(tp)
 treeptr tp;
{
 register boolean R173;
 treeptr tq;

 if (tp == (struct S64 *)NIL)
  R173 = false;
 else {
  switch ((int)(tp->tt)) {
    case ninteger:  case nreal:  case nchar:  case nstring:
   R173 = true;
   break ;
    case nid:
   tq = idup(tp);
   if (tq == (struct S64 *)NIL)
    R173 = false;
   else
    R173 = (boolean)(Member((unsigned)(tq->tt), Conset[129]));
   break ;
    case nuplus:  case numinus:  case nnot:
   R173 = constant(tp->U.V42.texps);
   break ;
    default:
   R173 = false;
  }
 }
 return R173;
}

 boolean
variable(tp, fn)
 treeptr tp, fn;
{
 register boolean R174;
 treeptr tq;

 if (tp == (struct S64 *)NIL)
  R174 = false;
 else {
  switch ((int)(tp->tt)) {
    case nid:
   tq = idup(tp);
   if (tq == (struct S64 *)NIL)
    R174 = false;
   else
    R174 = (boolean)(Member((unsigned)(tq->tt), Conset[130]));
   break ;
    case nindex:  case nselect:  case nderef:
   R174 = true;
   break ;
    case ncall:
   tq = idup(tp->U.V30.tcall);
   R174 = (boolean)((tq == fn) && (tp->U.V30.taparm == (struct S64 *)NIL) && (fn != (struct S64 *)NIL) && (fn->tt == nfunc));
   break ;
    default:
   R174 = false;
  }
 }
 return R174;
}

 boolean
functionresult(tp, fn)
 treeptr tp, fn;
{
 register boolean R175;
 treeptr tq;

 R175 = false;
 if ((tp != (struct S64 *)NIL) && (fn != (struct S64 *)NIL))
  if (tp->tt == ncall) {
   tq = idup(tp->U.V30.tcall);
   if ((tq == fn) && (tp->U.V30.taparm == (struct S64 *)NIL))
    R175 = true;
  } else
   if (tp->tt == nid)
    if (idup(tp) == fn)
     R175 = true;
 return R175;
}

 treeptr
setbase(tp)
 treeptr tp;
{
 register treeptr R176;
 treeptr tq;

 tq = typeof(tp);
 if (tq->tt == nsetof)
  R176 = basetype(tq->U.V18.tof);
 else
  if (tp->tt == nset) {
   tq = tp->U.V42.texps;
   if (tq == (struct S64 *)NIL)
    R176 = (struct S64 *)NIL;
   else
    if (tq->tt == nempty)
     R176 = (struct S64 *)NIL;
    else
     if (tq->tt == nrange)
      R176 = basetype(tq->U.V41.texpl);
     else
      R176 = basetype(tq);
  } else
   R176 = (struct S64 *)NIL;
 return R176;
}

 boolean
setcompatible(tl, tr)
 treeptr tl, tr;
{
 register boolean R177;
 treeptr ta, tb;

 ta = setbase(tl);
 tb = setbase(tr);
 if ((ta == (struct S64 *)NIL) || (tb == (struct S64 *)NIL))
  R177 = true;
 else
  R177 = sametype(ta, tb);
 return R177;
}

 treeptr
callarg(tp, n)
 treeptr tp;
 integer n;
{
 register treeptr R178;

 while ((tp != (struct S64 *)NIL) && (n > 1)) {
  tp = tp->tnext;
  n = n - 1;
 }
 R178 = tp;
 return R178;
}

 integer
argcount(tp)
 treeptr tp;
{
 register integer R179;
 integer n;

 n = 0;
 while (tp != (struct S64 *)NIL) {
  n = n + 1;
  tp = tp->tnext;
 }
 R179 = n;
 return R179;
}

 void
firstformal(sp, fp, fi)
 treeptr sp;
 treeptr *fp, *fi;
{
 (*fp) = sp;
 if ((*fp) == (struct S64 *)NIL)
  (*fi) = (struct S64 *)NIL;
 else
  if (Member((unsigned)((*fp)->tt), Conset[131]))
   (*fi) = (*fp)->U.V14.tidl;
  else
   (*fi) = (*fp)->U.V15.tparid;
}

 void
nextformal(fp, fi)
 treeptr *fp, *fi;
{
 if ((*fp) != (struct S64 *)NIL) {
  if ((Member((unsigned)((*fp)->tt), Conset[132])) && ((*fi)->tnext != (struct S64 *)NIL))
   (*fi) = (*fi)->tnext;
  else {
   (*fp) = (*fp)->tnext;
   if ((*fp) == (struct S64 *)NIL)
    (*fi) = (struct S64 *)NIL;
   else
    if (Member((unsigned)((*fp)->tt), Conset[133]))
     (*fi) = (*fp)->U.V14.tidl;
    else
     (*fi) = (*fp)->U.V15.tparid;
  }
 }
}

 treeptr
subdecl(tp)
 treeptr tp;
{
 register treeptr R180;

 if (tp == (struct S64 *)NIL)
  R180 = (struct S64 *)NIL;
 else
  if (tp->tt == ncall)
   R180 = idup(tp->U.V30.tcall);
  else
   if (tp->tt == nid)
    R180 = idup(tp);
   else
    R180 = (struct S64 *)NIL;
 return R180;
}

 boolean
sameprofile(formal, actual)
 treeptr formal, actual;
{
 register boolean R181;
 treeptr fp, fi, ap, ai;
 boolean good;

 good = true;
 if ((formal == (struct S64 *)NIL) || (actual == (struct S64 *)NIL))
  good = false;
 else
  if ((formal->tt == nparproc) && !(Member((unsigned)(actual->tt), Conset[134])))
   good = false;
  else
   if ((formal->tt == nparfunc) && !(Member((unsigned)(actual->tt), Conset[135])))
    good = false;
 if (good && (formal->tt == nparfunc))
  if (actual->tt == nfunc)
   good = sametype(formal->U.V15.tpartyp, actual->U.V13.tfuntyp);
  else
   good = sametype(formal->U.V15.tpartyp, actual->U.V15.tpartyp);
 if (good) {
  firstformal(formal->U.V15.tparparm, &fp, &fi);
  if (Member((unsigned)(actual->tt), Conset[136]))
   firstformal(actual->U.V13.tsubpar, &ap, &ai);
  else
   firstformal(actual->U.V15.tparparm, &ap, &ai);
  while (good && (fi != (struct S64 *)NIL) && (ai != (struct S64 *)NIL)) {
   if (fp->tt != ap->tt)
    good = false;
   else
    if (Member((unsigned)(fp->tt), Conset[137]))
     good = identicaltype(fp->U.V14.tbind, ap->U.V14.tbind);
    else
     good = sameprofile(fp, ap);
   nextformal(&fp, &fi);
   nextformal(&ap, &ai);
  }
  if ((fi != (struct S64 *)NIL) || (ai != (struct S64 *)NIL))
   good = false;
 }
 R181 = good;
 return R181;
}

 void
checkset(tp, fn)
 treeptr tp, fn;
{
 treeptr tq, tb, tx;

 tb = (struct S64 *)NIL;
 tq = tp->U.V42.texps;
 while (tq != (struct S64 *)NIL) {
  if (tq->tt == nempty) {
   if (tq->tnext != (struct S64 *)NIL)
    semerror(tq, eoperand);
  } else
   if (tq->tt == nrange) {
    checkexpr(tq->U.V41.texpl, fn);
    checkexpr(tq->U.V41.texpr, fn);
    if (!ordinaltype(tq->U.V41.texpl))
     semerror(tq, eoperand);
    if (!sametype(tq->U.V41.texpl, tq->U.V41.texpr))
     semerror(tq, eoperand);
    tx = basetype(tq->U.V41.texpl);
    if (constant(tq->U.V41.texpl) && constant(tq->U.V41.texpr))
     if (cvalof(tq->U.V41.texpl) > cvalof(tq->U.V41.texpr))
      semerror(tq, ebadrange);
   } else {
    checkexpr(tq, fn);
    if (!ordinaltype(tq))
     semerror(tq, eoperand);
    tx = basetype(tq);
   }
  if (tq->tt != nempty)
   if (tb == (struct S64 *)NIL)
    tb = tx;
   else
    if (!sametype(tb, tx))
     semerror(tq, eoperand);
  tq = tq->tnext;
 }
}

 void
checkpredef(pd, cp, fn)
 predefs pd;
 treeptr cp, fn;
{
 treeptr a, f, tq;
 integer n;

 a = cp->U.V30.taparm;
 n = argcount(a);
 switch ((int)(pd)) {
   case dabs:  case dsqr:
  if (n != 1)
   semerror(cp, eargcount);
  checkexpr(a, fn);
  if (!numerictype(a))
   semerror(a, eargtype);
  break ;
   case darctan:  case dcos:  case dexp:  case dln:
   case dsin:  case dsqrt:  case dtan:
  if (n != 1)
   semerror(cp, eargcount);
  checkexpr(a, fn);
  if (!numerictype(a))
   semerror(a, eargtype);
  break ;
   case dchr:
  if (n != 1)
   semerror(cp, eargcount);
  checkexpr(a, fn);
  if (!ordinaltype(a))
   semerror(a, eargtype);
  break ;
   case dord:  case dpred:  case dsucc:
  if (n != 1)
   semerror(cp, eargcount);
  checkexpr(a, fn);
  if (!ordinaltype(a))
   semerror(a, eargtype);
  break ;
   case dodd:
  if (n != 1)
   semerror(cp, eargcount);
  checkexpr(a, fn);
  if (!integertype(a))
   semerror(a, eargtype);
  break ;
   case dround:  case dtrunc:
  if (n != 1)
   semerror(cp, eargcount);
  checkexpr(a, fn);
  if (!numerictype(a))
   semerror(a, eargtype);
  break ;
   case deof:  case deoln:  case dflush:  case dpage:
  if (n > 1)
   semerror(cp, eargcount);
  if (a != (struct S64 *)NIL) {
   checkexpr(a, fn);
   if (!filetype(a))
    semerror(a, eargtype);
  }
  break ;
   case dexit:
  if (n > 1)
   semerror(cp, eargcount);
  if (a != (struct S64 *)NIL) {
   checkexpr(a, fn);
   if (!integertype(a))
    semerror(a, eargtype);
  }
  break ;
   case dhalt:
  if (n != 0)
   semerror(cp, eargcount);
  break ;
   case dget:  case dput:  case dclose:
  if (n != 1)
   semerror(cp, eargcount);
  checkexpr(a, fn);
  if (!filetype(a))
   semerror(a, eargtype);
  if (!variable(a, fn))
   semerror(a, eidkind);
  break ;
   case ddispose:
  if (n != 1)
   semerror(cp, eargcount);
  checkexpr(a, fn);
  if (typeof(a)->tt != nptr)
   semerror(a, eargtype);
  break ;
   case dnew:
  if (n < 1)
   semerror(cp, eargcount);
  checkexpr(a, fn);
  if (!variable(a, fn))
   semerror(a, eidkind);
  if (typeof(a)->tt != nptr)
   semerror(a, eargtype);
  tq = a->tnext;
  while (tq != (struct S64 *)NIL) {
   checkexpr(tq, fn);
   if (!constant(tq))
    semerror(tq, econstexpected);
   tq = tq->tnext;
  }
  break ;
   case dargv:
  if (n != 2)
   semerror(cp, eargcount);
  checkexpr(a, fn);
  if (!integertype(a))
   semerror(a, eargtype);
  tq = a->tnext;
  checkexpr(tq, fn);
  if (!variable(tq, fn))
   semerror(tq, eidkind);
  if (!chararray(tq))
   semerror(tq, eargtype);
  break ;
   case dreset:  case drewrite:
  if ((n < 1) || (n > 2))
   semerror(cp, eargcount);
  checkexpr(a, fn);
  if (!filetype(a))
   semerror(a, eargtype);
  if (!variable(a, fn))
   semerror(a, eidkind);
  tq = a->tnext;
  if (tq != (struct S64 *)NIL) {
   checkexpr(tq, fn);
   if (!((basetype(tq) == typnods.A[(int)(tchar)]) || (typeof(tq) == typnods.A[(int)(tstring)]) || chararray(tq)))
    semerror(tq, eargtype);
  }
  break ;
   case dread:  case dreadln:
  tq = a;
  if (tq != (struct S64 *)NIL)
   if (filetype(tq)) {
    checkexpr(tq, fn);
    tq = tq->tnext;
   }
  while (tq != (struct S64 *)NIL) {
   if (tq->tt == nformat)
    f = tq->U.V41.texpl;
   else
    f = tq;
   checkexpr(f, fn);
   if (!variable(f, fn))
    semerror(f, eidkind);
   tq = tq->tnext;
  }
  break ;
   case dwrite:  case dwriteln:  case dmessage:
  tq = a;
  if ((pd != dmessage) && (tq != (struct S64 *)NIL))
   if (filetype(tq)) {
    checkexpr(tq, fn);
    tq = tq->tnext;
   }
  while (tq != (struct S64 *)NIL) {
   checkexpr(tq, fn);
   f = typeof(tq);
   if (tq->tt == nformat)
    f = typeof(tq->U.V41.texpl);
   if (!((basetype(f) == typnods.A[(int)(tinteger)]) || (basetype(f) == typnods.A[(int)(tchar)]) || (basetype(f) == typnods.A[(int)(treal)]) || (basetype(f) == typnods.A[(int)(tboolean)]) || (f == typnods.A[(int)(tstring)]) || (f->tt == narray) || (f->tt == nconfarr)))
    semerror(tq, eargtype);
   tq = tq->tnext;
  }
  break ;
   case dpack:
  if (n != 3)
   semerror(cp, eargcount);
  tq = callarg(a, 1);
  checkexpr(tq, fn);
  if (!(Member((unsigned)(typeof(tq)->tt), Conset[138])))
   semerror(tq, eargtype);
  tq = callarg(a, 2);
  checkexpr(tq, fn);
  if (!ordinaltype(tq))
   semerror(tq, eargtype);
  tq = callarg(a, 3);
  checkexpr(tq, fn);
  if (!variable(tq, fn))
   semerror(tq, eidkind);
  if (!(Member((unsigned)(typeof(tq)->tt), Conset[139])))
   semerror(tq, eargtype);
  break ;
   case dunpack:
  if (n != 3)
   semerror(cp, eargcount);
  tq = callarg(a, 1);
  checkexpr(tq, fn);
  if (!(Member((unsigned)(typeof(tq)->tt), Conset[140])))
   semerror(tq, eargtype);
  tq = callarg(a, 2);
  checkexpr(tq, fn);
  if (!variable(tq, fn))
   semerror(tq, eidkind);
  if (!(Member((unsigned)(typeof(tq)->tt), Conset[141])))
   semerror(tq, eargtype);
  tq = callarg(a, 3);
  checkexpr(tq, fn);
  if (!ordinaltype(tq))
   semerror(tq, eargtype);
  break ;
   default:
{
   tq = a;
   while (tq != (struct S64 *)NIL) {
    checkexpr(tq, fn);
    tq = tq->tnext;
   }
  }
 }
}

 void
checkcall(cp, fn, expression)
 treeptr cp, fn;
 boolean expression;
{
 treeptr sd, fp, fi, ap, ad;
 predefs pd;

 if ((cp->U.V30.tcall == (struct S64 *)NIL) || (cp->U.V30.tcall->tt != nid))
  semerror(cp, ecalltarget);
 sd = idup(cp->U.V30.tcall);
 if (sd == (struct S64 *)NIL)
  semerror(cp, ecalltarget);
 if (!(Member((unsigned)(sd->tt), Conset[142])))
  semerror(cp, ecalltarget);
 if (expression) {
  if (Member((unsigned)(sd->tt), Conset[143]))
   semerror(cp, eprocexpr);
 } else
  if (Member((unsigned)(sd->tt), Conset[144]))
   semerror(cp, efuncstmt);
 if ((Member((unsigned)(sd->tt), Conset[145])) && (sd->U.V13.tsubstmt != (struct S64 *)NIL) && (sd->U.V13.tsubstmt->tt == npredef)) {
  pd = sd->U.V13.tsubstmt->U.V12.tdef;
  checkpredef(pd, cp, fn);
 } else {
  if (Member((unsigned)(sd->tt), Conset[146]))
   firstformal(sd->U.V13.tsubpar, &fp, &fi);
  else
   firstformal(sd->U.V15.tparparm, &fp, &fi);
  ap = cp->U.V30.taparm;
  while ((fi != (struct S64 *)NIL) && (ap != (struct S64 *)NIL)) {
   if (Member((unsigned)(fp->tt), Conset[147])) {
    checkexpr(ap, fn);
    if (fp->tt == nvarpar) {
     if (!variable(ap, fn))
      semerror(ap, evarpar);
     if (!identicaltype(fp->U.V14.tbind, ap))
      semerror(ap, eargtype);
    } else
     if (!assignable(fp->U.V14.tbind, ap))
      semerror(ap, eargtype);
   } else {
    ad = subdecl(ap);
    if (ad == (struct S64 *)NIL)
     semerror(ap, eargtype);
    if (!sameprofile(fp, ad))
     semerror(ap, eargtype);
   }
   nextformal(&fp, &fi);
   ap = ap->tnext;
  }
  if ((fi != (struct S64 *)NIL) || (ap != (struct S64 *)NIL))
   semerror(cp, eargcount);
 }
}

 void
checkexpr(tp, fn)
 treeptr tp, fn;
{
 treeptr tl, tr, tq, ti;

 if (tp == (struct S64 *)NIL)
  semerror(tp, eoperand);
 switch ((int)(tp->tt)) {
   case nid:
  tq = idup(tp);
  if (tq == (struct S64 *)NIL)
   semerror(tp, eidkind);
  if (!(Member((unsigned)(tq->tt), Conset[148])))
   semerror(tp, eidkind);
  break ;
   case ninteger:  case nreal:  case nchar:  case nstring:
   case nnil:
  break ;
   case ncall:
  checkcall(tp, fn, true);
  break ;
   case nuplus:
  checkexpr(tp->U.V42.texps, fn);
  if (tp->U.V42.tuplusop)
   if (!numerictype(tp->U.V42.texps))
    semerror(tp, eoperand);
  break ;
   case numinus:
  checkexpr(tp->U.V42.texps, fn);
  if (!numerictype(tp->U.V42.texps))
   semerror(tp, eoperand);
  break ;
   case nnot:
  checkexpr(tp->U.V42.texps, fn);
  if (basetype(tp->U.V42.texps) != typnods.A[(int)(tboolean)])
   semerror(tp, eboolxp);
  break ;
   case nplus:  case nminus:  case nmul:
  checkexpr(tp->U.V41.texpl, fn);
  checkexpr(tp->U.V41.texpr, fn);
  tl = typeof(tp->U.V41.texpl);
  tr = typeof(tp->U.V41.texpr);
  if (numerictype(tl) && numerictype(tr))
   ;
  else
   if (((tl->tt == nsetof) || (tl == typnods.A[(int)(tset)])) && ((tr->tt == nsetof) || (tr == typnods.A[(int)(tset)]))) {
    if (!setcompatible(tp->U.V41.texpl, tp->U.V41.texpr))
     semerror(tp, eoperand);
   } else
    semerror(tp, eoperand);
  break ;
   case ndiv:  case nmod:
  checkexpr(tp->U.V41.texpl, fn);
  checkexpr(tp->U.V41.texpr, fn);
  if (!integertype(tp->U.V41.texpl) || !integertype(tp->U.V41.texpr))
   semerror(tp, eoperand);
  break ;
   case nquot:
  checkexpr(tp->U.V41.texpl, fn);
  checkexpr(tp->U.V41.texpr, fn);
  if (!numerictype(tp->U.V41.texpl) || !numerictype(tp->U.V41.texpr))
   semerror(tp, eoperand);
  break ;
   case nand:  case nor:
  checkexpr(tp->U.V41.texpl, fn);
  checkexpr(tp->U.V41.texpr, fn);
  if ((basetype(tp->U.V41.texpl) != typnods.A[(int)(tboolean)]) || (basetype(tp->U.V41.texpr) != typnods.A[(int)(tboolean)]))
   semerror(tp, eboolxp);
  break ;
   case neq:  case nne:  case nlt:  case nle:
   case ngt:  case nge:
  checkexpr(tp->U.V41.texpl, fn);
  checkexpr(tp->U.V41.texpr, fn);
  tl = typeof(tp->U.V41.texpl);
  tr = typeof(tp->U.V41.texpr);
  if (numerictype(tl) && numerictype(tr))
   ;
  else
   if (ordinaltype(tl) && ordinaltype(tr) && sametype(tl, tr))
    ;
   else
    if (((tl->tt == nptr) && (tr == typnods.A[(int)(tnil)])) || ((tr->tt == nptr) && (tl == typnods.A[(int)(tnil)])) || ((tl->tt == nptr) && sametype(tl, tr))) {
     if (!(Member((unsigned)(tp->tt), Conset[149])))
      semerror(tp, eoperand);
    } else
     if (((tl->tt == nsetof) || (tl == typnods.A[(int)(tset)])) && ((tr->tt == nsetof) || (tr == typnods.A[(int)(tset)]))) {
      if (!(Member((unsigned)(tp->tt), Conset[150])))
       semerror(tp, eoperand);
      if (!setcompatible(tp->U.V41.texpl, tp->U.V41.texpr))
       semerror(tp, eoperand);
     } else
      if (((tl == typnods.A[(int)(tstring)]) || chararray(tl)) && ((tr == typnods.A[(int)(tstring)]) || chararray(tr)))
       ;
      else
       semerror(tp, eoperand);
  break ;
   case nin:
  checkexpr(tp->U.V41.texpl, fn);
  checkexpr(tp->U.V41.texpr, fn);
  if (!ordinaltype(tp->U.V41.texpl))
   semerror(tp, eoperand);
  tr = typeof(tp->U.V41.texpr);
  if (!((tr->tt == nsetof) || (tr == typnods.A[(int)(tset)])))
   semerror(tp, eoperand);
  tq = setbase(tp->U.V41.texpr);
  if (tq != (struct S64 *)NIL)
   if (!sametype(tp->U.V41.texpl, tq))
    semerror(tp, eoperand);
  break ;
   case nset:
  checkset(tp, fn);
  break ;
   case nrange:
  semerror(tp, eoperand);
  break ;
   case nformat:
  checkexpr(tp->U.V41.texpl, fn);
  checkexpr(tp->U.V41.texpr, fn);
  if (!integertype(tp->U.V41.texpr))
   semerror(tp, eargtype);
  break ;
   case nindex:
  checkexpr(tp->U.V39.tvariable, fn);
  checkexpr(tp->U.V39.toffset, fn);
  tq = typeof(tp->U.V39.tvariable);
  if (tq->tt == narray)
   ti = tq->U.V23.taindx;
  else
   if (tq->tt == nconfarr)
    ti = tq->U.V22.tindtyp;
   else
    if (tq == typnods.A[(int)(tstring)])
     ti = typnods.A[(int)(tinteger)];
    else
     semerror(tp, eindex);
  if (!ordinaltype(tp->U.V39.toffset))
   semerror(tp, eindex);
  if (!sametype(ti, tp->U.V39.toffset))
   semerror(tp, eindex);
  break ;
   case nselect:
  checkexpr(tp->U.V40.trecord, fn);
  tq = typeof(tp->U.V40.trecord);
  if (tq->tt != nrecord)
   semerror(tp, enotrecord);
  if (declof(tp->U.V40.tfield)->tt != nfield)
   semerror(tp, eidkind);
  break ;
   case nderef:
  checkexpr(tp->U.V42.texps, fn);
  tq = typeof(tp->U.V42.texps);
  if (!((tq->tt == nptr) || (tq->tt == nfileof) || (tq == typnods.A[(int)(ttext)])))
   semerror(tp, enotpointer);
  break ;
   case nempty:
  semerror(tp, eoperand);
  break ;
   default:
  semerror(tp, eoperand);
 }
 tq = typeof(tp);
}

 void
checktype(tp)
 treeptr tp;
{
 treeptr tq, ti, tb;

 if (tp == (struct S64 *)NIL)
  semerror(tp, etypeexpected);
 if (!tp->tsemchecked) {
  if (!tp->tsemchecking) {
   tp->tsemchecking = true;
   switch ((int)(tp->tt)) {
     case nid:
    tq = idup(tp);
    if ((tq == (struct S64 *)NIL) || (tq->tt != ntype))
     semerror(tp, etypeexpected);
    checktype(tq->U.V14.tbind);
    break ;
     case npredef:
    break ;
     case nptr:
    checktype(tp->U.V16.tptrid);
    break ;
     case nscalar:
    break ;
     case nsubrange:
    if (!constant(tp->U.V19.tlo) || !constant(tp->U.V19.thi))
     semerror(tp, econstexpected);
    if (!ordinaltype(tp->U.V19.tlo) || !ordinaltype(tp->U.V19.thi))
     semerror(tp, ebadrange);
    if (!sametype(tp->U.V19.tlo, tp->U.V19.thi))
     semerror(tp, ebadrange);
    if (cvalof(tp->U.V19.tlo) > cvalof(tp->U.V19.thi))
     semerror(tp, ebadrange);
    break ;
     case narray:
    checktype(tp->U.V23.taindx);
    if (!ordinaltype(tp->U.V23.taindx))
     semerror(tp, eindex);
    checktype(tp->U.V23.taelem);
    break ;
     case nconfarr:
    checktype(tp->U.V22.tindtyp);
    if (!ordinaltype(tp->U.V22.tindtyp))
     semerror(tp, eindex);
    checktype(tp->U.V22.tcelem);
    break ;
     case nsetof:
    checktype(tp->U.V18.tof);
    if (!ordinaltype(tp->U.V18.tof))
     semerror(tp, eoperand);
    break ;
     case nfileof:
    checktype(tp->U.V18.tof);
    break ;
     case nrecord:
    tq = tp->U.V21.tflist;
    while (tq != (struct S64 *)NIL) {
     if (!typeref(tq->U.V14.tbind))
      semerror(tq, etypeexpected);
     checktype(tq->U.V14.tbind);
     tq = tq->tnext;
    }
    tq = tp->U.V21.tvlist;
    tb = (struct S64 *)NIL;
    while (tq != (struct S64 *)NIL) {
     ti = tq->U.V20.tselct;
     while (ti != (struct S64 *)NIL) {
      if (!constant(ti))
       semerror(ti, econstexpected);
      if (!ordinaltype(ti))
       semerror(ti, ecasetype);
      if (tb == (struct S64 *)NIL)
       tb = basetype(ti);
      else
       if (!sametype(tb, ti))
        semerror(ti, ecasetype);
      ti = ti->tnext;
     }
     checktype(tq->U.V20.tvrnt);
     tq = tq->tnext;
    }
    break ;
     default:
    semerror(tp, etypeexpected);
   }
   tp->tsemchecking = false;
   tp->tsemchecked = true;
  }
 }
}

 void
checktypedecls(tp)
 treeptr tp;
{
 while (tp != (struct S64 *)NIL) {
  if (!typeref(tp->U.V14.tbind))
   semerror(tp, etypeexpected);
  checktype(tp->U.V14.tbind);
  tp = tp->tnext;
 }
}

 void
checkconstants(tp, fn)
 treeptr tp, fn;
{
 while (tp != (struct S64 *)NIL) {
  if (!constant(tp->U.V14.tbind))
   semerror(tp, econstexpected);
  checkexpr(tp->U.V14.tbind, fn);
  tp = tp->tnext;
 }
}

 void
checkparams(tp)
 treeptr tp;
{
 while (tp != (struct S64 *)NIL) {
  switch ((int)(tp->tt)) {
    case nvalpar:  case nvarpar:
   if (!typeref(tp->U.V14.tbind))
    semerror(tp, etypeexpected);
   checktype(tp->U.V14.tbind);
   break ;
    case nparproc:
   checkparams(tp->U.V15.tparparm);
   break ;
    case nparfunc:
   checkparams(tp->U.V15.tparparm);
   if (!typeref(tp->U.V15.tpartyp))
    semerror(tp, efunrtype);
   checktype(tp->U.V15.tpartyp);
   break ;
    default:
   Caseerror(Line);
  }
  tp = tp->tnext;
 }
}

 boolean
resulttype(tp)
 treeptr tp;
{
 register boolean R182;
 treeptr tq;

 tq = typeof(tp);
 R182 = (boolean)(!(Member((unsigned)(tq->tt), Conset[151])));
 return R182;
}

 void
checkcase(tp, fn, assigned)
 treeptr tp, fn;
 boolean *assigned;
{
 treeptr tq, ti, cq, ci;
 treeptr st;

 checkexpr(tp->U.V35.tcasxp, fn);
 st = basetype(tp->U.V35.tcasxp);
 if (!ordinaltype(st))
  semerror(tp, ecasetype);
 tq = tp->U.V35.tcaslst;
 while (tq != (struct S64 *)NIL) {
  ti = tq->U.V36.tchocon;
  while (ti != (struct S64 *)NIL) {
   if (!constant(ti))
    semerror(ti, econstexpected);
   checkexpr(ti, fn);
   if (!sametype(st, ti))
    semerror(ti, ecasetype);
   cq = tp->U.V35.tcaslst;
   while (cq != tq) {
    ci = cq->U.V36.tchocon;
    while (ci != (struct S64 *)NIL) {
     if (cvalof(ci) == cvalof(ti))
      semerror(ti, edupcase);
     ci = ci->tnext;
    }
    cq = cq->tnext;
   }
   ci = tq->U.V36.tchocon;
   while (ci != ti) {
    if (cvalof(ci) == cvalof(ti))
     semerror(ti, edupcase);
    ci = ci->tnext;
   }
   ti = ti->tnext;
  }
  checkstmt(tq->U.V36.tchostmt, fn, &(*assigned));
  tq = tq->tnext;
 }
 checkstmt(tp->U.V35.tcasother, fn, &(*assigned));
}

 void
checkstmt(tp, fn, assigned)
 treeptr tp, fn;
 boolean *assigned;
{
 treeptr tq, ti, ct;

 while (tp != (struct S64 *)NIL) {
  switch ((int)(tp->tt)) {
    case nempty:  case ngoto:
   break ;
    case nlabstmt:
   checkstmt(tp->U.V25.tstmt, fn, &(*assigned));
   break ;
    case nbegin:
   checkstmt(tp->U.V24.tbegin, fn, &(*assigned));
   break ;
    case nassign:
   checkexpr(tp->U.V27.trhs, fn);
   if (functionresult(tp->U.V27.tlhs, fn)) {
    if (!assignable(fn->U.V13.tfuntyp, tp->U.V27.trhs))
     semerror(tp, easgntype);
    (*assigned) = true;
   } else {
    if (!variable(tp->U.V27.tlhs, fn))
     semerror(tp->U.V27.tlhs, eidkind);
    checkexpr(tp->U.V27.tlhs, fn);
    if (!assignable(tp->U.V27.tlhs, tp->U.V27.trhs))
     semerror(tp, easgntype);
   }
   break ;
    case ncall:
   checkcall(tp, fn, false);
   break ;
    case nif:
   checkexpr(tp->U.V31.tifxp, fn);
   if (basetype(tp->U.V31.tifxp) != typnods.A[(int)(tboolean)])
    semerror(tp, eboolxp);
   checkstmt(tp->U.V31.tthen, fn, &(*assigned));
   checkstmt(tp->U.V31.telse, fn, &(*assigned));
   break ;
    case nwhile:
   checkexpr(tp->U.V32.twhixp, fn);
   if (basetype(tp->U.V32.twhixp) != typnods.A[(int)(tboolean)])
    semerror(tp, eboolxp);
   checkstmt(tp->U.V32.twhistmt, fn, &(*assigned));
   break ;
    case nrepeat:
   checkstmt(tp->U.V33.treptstmt, fn, &(*assigned));
   checkexpr(tp->U.V33.treptxp, fn);
   if (basetype(tp->U.V33.treptxp) != typnods.A[(int)(tboolean)])
    semerror(tp, eboolxp);
   break ;
    case nfor:
   if (!variable(tp->U.V34.tforid, fn))
    semerror(tp, eforctrl);
   checkexpr(tp->U.V34.tforid, fn);
   checkexpr(tp->U.V34.tfrom, fn);
   checkexpr(tp->U.V34.tto, fn);
   ct = typeof(tp->U.V34.tforid);
   if (!ordinaltype(ct))
    semerror(tp, eforctrl);
   if (!assignable(ct, tp->U.V34.tfrom) || !assignable(ct, tp->U.V34.tto))
    semerror(tp, eforctrl);
   checkstmt(tp->U.V34.tforstmt, fn, &(*assigned));
   break ;
    case ncase:
   checkcase(tp, fn, &(*assigned));
   break ;
    case nwith:
   tq = tp->U.V37.twithvar;
   while (tq != (struct S64 *)NIL) {
    checkexpr(tq->U.V38.texpw, fn);
    if (typeof(tq->U.V38.texpw)->tt != nrecord)
     semerror(tq, enotrecord);
    if (!variable(tq->U.V38.texpw, fn))
     semerror(tq, eidkind);
    tq = tq->tnext;
   }
   checkstmt(tp->U.V37.twithstmt, fn, &(*assigned));
   break ;
    default:
   semerror(tp, eidkind);
  }
  tp = tp->tnext;
 }
}

 void
checksub(tp)
 treeptr tp;
{
 treeptr tq;
 boolean assigned;

 while (tp != (struct S64 *)NIL) {
  checkparams(tp->U.V13.tsubpar);
  if (tp->tt == nfunc) {
   if (!typeref(tp->U.V13.tfuntyp))
    semerror(tp, efunrtype);
   checktype(tp->U.V13.tfuntyp);
   if (!resulttype(tp->U.V13.tfuntyp))
    semerror(tp, efunrtype);
  }
  checkconstants(tp->U.V13.tsubconst, tp);
  checktypedecls(tp->U.V13.tsubtype);
  checktypedecls(tp->U.V13.tsubvar);
  checksub(tp->U.V13.tsubsub);
  assigned = false;
  checkstmt(tp->U.V13.tsubstmt, tp, &assigned);
  if ((tp->tt == nfunc) && (tp->U.V13.tsubstmt != (struct S64 *)NIL))
   if (!assigned)
    semerror(tp, enofunresult);
  tp = tp->tnext;
 }
}

 void
semcheck()
{
 boolean assigned;

 checkconstants(top->U.V13.tsubconst, top);
 checktypedecls(top->U.V13.tsubtype);
 checktypedecls(top->U.V13.tsubvar);
 checksub(top->U.V13.tsubsub);
 assigned = false;
 checkstmt(top->U.V13.tsubstmt, top, &assigned);
}

 integer
cvalof(tp)
 treeptr tp;
{
 register integer R183;
 integer v;
 treeptr tq;

 switch ((int)(tp->tt)) {
   case nuplus:
  R183 = cvalof(tp->U.V42.texps);
  break ;
   case numinus:
  R183 = -cvalof(tp->U.V42.texps);
  break ;
   case nnot:
  R183 = 1 - cvalof(tp->U.V42.texps);
  break ;
   case nid:
  tq = idup(tp);
  if (tq == (struct S64 *)NIL)
   fatal(etree);
  tp = tp->U.V43.tsym->lsymdecl;
  switch ((int)(tq->tt)) {
    case nscalar:
   v = 0;
   tq = tq->U.V17.tscalid;
   while (tq != (struct S64 *)NIL)
    if (tq == tp)
     tq = (struct S64 *)NIL;
    else {
     v = v + 1;
     tq = tq->tnext;
    }
   R183 = v;
   break ;
    case nconst:
   R183 = cvalof(tq->U.V14.tbind);
   break ;
    default:
   Caseerror(Line);
  }
  break ;
   case ninteger:
  R183 = tp->U.V43.tsym->U.V10.linum;
  break ;
   case nchar:
  R183 = (unsigned)(tp->U.V43.tsym->U.V11.lchar);
  break ;
   default:
  Caseerror(Line);
 }
 return R183;
}

 integer
clower(tp)
 treeptr tp;
{
 register integer R184;
 treeptr tq;

 tq = typeof(tp);
 if (tq->tt == nscalar)
  R184 = scalbase;
 else
  if (tq->tt == nsubrange)
   if (tq->tup->tt == nconfarr)
    R184 = 0;
   else
    R184 = cvalof(tq->U.V19.tlo);
  else
   if (tq == typnods.A[(int)(tchar)])
    R184 = 0;
   else
    if (tq == typnods.A[(int)(tinteger)])
     R184 = -maxint;
    else
     fatal(etree);
 return R184;
}

 integer
cupper(tp)
 treeptr tp;
{
 register integer R185;
 treeptr tq;
 integer i;

 tq = typeof(tp);
 if (tq->tt == nscalar) {
  tq = tq->U.V17.tscalid;
  i = scalbase;
  while (tq->tnext != (struct S64 *)NIL) {
   i = i + 1;
   tq = tq->tnext;
  }
  R185 = i;
 } else
  if (tq->tt == nsubrange)
   if (tq->tup->tt == nconfarr)
    fatal(euprconf);
   else
    R185 = cvalof(tq->U.V19.thi);
  else
   if (tq == typnods.A[(int)(tchar)])
    R185 = maxchar;
   else
    if (tq == typnods.A[(int)(tinteger)])
     R185 = maxint;
    else
     fatal(etree);
 return R185;
}

 integer
crange(tp)
 treeptr tp;
{
 register integer R186;

 R186 = cupper(tp) - clower(tp) + 1;
 return R186;
}

 integer
csetwords(i)
 integer i;
{
 register integer R187;

 i = (i + (C37_setbits)) / (C37_setbits + 1);
 if (i > maxsetrange)
  error(esetsize);
 R187 = i;
 return R187;
}

 integer
csetsize(tp)
 treeptr tp;
{
 register integer R188;
 treeptr tq;
 integer i;

 tq = typeof(tp->U.V18.tof);
 i = clower(tq);
 if ((i < 0) || (i >= 6 * (C37_setbits + 1)))
  error(esetbase);
 R188 = csetwords(crange(tq)) + 1;
 return R188;
}

 boolean
islocal(tp)
 treeptr tp;
{
 register boolean R189;
 treeptr tq;

 tq = tp->U.V43.tsym->lsymdecl;
 while (!(Member((unsigned)(tq->tt), Conset[152])))
  tq = tq->tup;
 while (!(Member((unsigned)(tp->tt), Conset[153])))
  tp = tp->tup;
 R189 = (boolean)(tp == tq);
 return R189;
}

void transform();

void renamf();

 void
crtnvar(tp)
 treeptr tp;
{
 while (tp != (struct S64 *)NIL) {
  switch ((int)(tp->tt)) {
    case npgm:
   crtnvar(tp->U.V13.tsubsub);
   break ;
    case nfunc:  case nproc:
   crtnvar(tp->U.V13.tsubsub);
   crtnvar(tp->U.V13.tsubstmt);
   break ;
    case nbegin:
   crtnvar(tp->U.V24.tbegin);
   break ;
    case nif:
   crtnvar(tp->U.V31.tthen);
   crtnvar(tp->U.V31.telse);
   break ;
    case nwhile:
   crtnvar(tp->U.V32.twhistmt);
   break ;
    case nrepeat:
   crtnvar(tp->U.V33.treptstmt);
   break ;
    case nfor:
   crtnvar(tp->U.V34.tforstmt);
   break ;
    case ncase:
   crtnvar(tp->U.V35.tcaslst);
   crtnvar(tp->U.V35.tcasother);
   break ;
    case nchoise:
   crtnvar(tp->U.V36.tchostmt);
   break ;
    case nwith:
   crtnvar(tp->U.V37.twithstmt);
   break ;
    case nlabstmt:
   crtnvar(tp->U.V25.tstmt);
   break ;
    case nassign:
   if (tp->U.V27.tlhs->tt == ncall) {
    tp->U.V27.tlhs = tp->U.V27.tlhs->U.V30.tcall;
    tp->U.V27.tlhs->tup = tp;
   }
   (*G213_tv) = tp->U.V27.tlhs;
   if ((*G213_tv)->tt == nid)
    if ((*G213_tv)->U.V43.tsym == (*G209_ip))
     (*G213_tv)->U.V43.tsym = (*G211_iq);
   break ;
    case nbreak:  case npush:  case npop:  case ngoto:
    case nempty:  case ncall:
   break ;
    default:
   Caseerror(Line);
  }
  tp = tp->tnext;
 }
}

 void
renamf(tp)
 treeptr tp;
{
 symptr ip, iq;
 treeptr tq, tv;
 symptr *F210;
 symptr *F212;
 treeptr *F214;

 F214 = G213_tv;
 G213_tv = &tv;
 F212 = G211_iq;
 G211_iq = &iq;
 F210 = G209_ip;
 G209_ip = &ip;
 while (tp != (struct S64 *)NIL) {
  switch ((int)(tp->tt)) {
    case npgm:  case nproc:
   renamf(tp->U.V13.tsubsub);
   break ;
    case nfunc:
   tq = mknode(nvar);
   tq->U.V14.tattr = aregister;
   tq->tup = tp;
   tq->U.V14.tidl = newid(mkvariable('R'));
   tq->U.V14.tidl->tup = tq;
   tq->U.V14.tbind = tp->U.V13.tfuntyp;
   tq->tnext = tp->U.V13.tsubvar;
   tp->U.V13.tsubvar = tq;
   (*G211_iq) = tq->U.V14.tidl->U.V43.tsym;
   (*G209_ip) = tp->U.V13.tsubid->U.V43.tsym;
   crtnvar(tp->U.V13.tsubsub);
   crtnvar(tp->U.V13.tsubstmt);
   renamf(tp->U.V13.tsubsub);
   break ;
    default:
   Caseerror(Line);
  }
  tp = tp->tnext;
 }
 G209_ip = F210;
 G211_iq = F212;
 G213_tv = F214;
}

void extract();

 treeptr
xtrit(tp, pp, last)
 treeptr tp, pp;
 boolean last;
{
 register treeptr R190;
 treeptr np, rp;
 idptr ip;

 np = mknode(ntype);
 ip = mkvariable('T');
 np->U.V14.tidl = newid(ip);
 np->U.V14.tidl->tup = np;
 rp = oldid(ip, lidentifier);
 rp->tup = tp->tup;
 rp->tnext = tp->tnext;
 np->U.V14.tbind = tp;
 tp->tup = np;
 tp->tnext = (struct S64 *)NIL;
 np->tup = pp;
 if (last && (pp->U.V13.tsubtype != (struct S64 *)NIL)) {
  pp = pp->U.V13.tsubtype;
  while (pp->tnext != (struct S64 *)NIL)
   pp = pp->tnext;
  pp->tnext = np;
 } else {
  np->tnext = pp->U.V13.tsubtype;
  pp->U.V13.tsubtype = np;
 }
 R190 = rp;
 return R190;
}

treeptr xtrenum();

 void
nametype(tp)
 treeptr tp;
{
 tp = typeof(tp);
 if (tp->tt == nrecord)
  if (tp->U.V21.tuid == (struct S62 *)NIL)
   tp->U.V21.tuid = mkvariable('S');
}

 treeptr
xtrenum(tp, pp)
 treeptr tp, pp;
{
 register treeptr R191;

 if (tp != (struct S64 *)NIL) {
  switch ((int)(tp->tt)) {
    case nfield:  case ntype:  case nvar:
   tp->U.V14.tbind = xtrenum(tp->U.V14.tbind, pp);
   break ;
    case nscalar:
   if (tp->tup->tt != ntype)
    tp = xtrit(tp, pp, false);
   break ;
    case narray:
   tp->U.V23.taindx = xtrenum(tp->U.V23.taindx, pp);
   tp->U.V23.taelem = xtrenum(tp->U.V23.taelem, pp);
   break ;
    case nrecord:
   tp->U.V21.tflist = xtrenum(tp->U.V21.tflist, pp);
   tp->U.V21.tvlist = xtrenum(tp->U.V21.tvlist, pp);
   break ;
    case nvariant:
   tp->U.V20.tvrnt = xtrenum(tp->U.V20.tvrnt, pp);
   break ;
    case nfileof:
   tp->U.V18.tof = xtrenum(tp->U.V18.tof, pp);
   break ;
    case nptr:
   nametype(tp->U.V16.tptrid);
   break ;
    case nid:  case nsubrange:  case npredef:  case nempty:
    case nsetof:
   break ;
    default:
   Caseerror(Line);
  }
  tp->tnext = xtrenum(tp->tnext, pp);
 }
 R191 = tp;
 return R191;
}

 void
extract(tp)
 treeptr tp;
{
 treeptr vp;

 while (tp != (struct S64 *)NIL) {
  tp->U.V13.tsubtype = xtrenum(tp->U.V13.tsubtype, tp);
  tp->U.V13.tsubvar = xtrenum(tp->U.V13.tsubvar, tp);
  vp = tp->U.V13.tsubvar;
  while (vp != (struct S64 *)NIL) {
   if (Member((unsigned)(vp->U.V14.tbind->tt), Conset[154]))
    vp->U.V14.tbind = xtrit(vp->U.V14.tbind, tp, true);
   vp = vp->tnext;
  }
  extract(tp->U.V13.tsubsub);
  tp = tp->tnext;
 }
}

void global();

 void
markdecl(xp)
 treeptr xp;
{
 while (xp != (struct S64 *)NIL) {
  switch ((int)(xp->tt)) {
    case nid:
   xp->U.V43.tsym->U.V6.lused = false;
   break ;
    case nconst:
   markdecl(xp->U.V14.tidl);
   break ;
    case ntype:  case nvar:  case nvalpar:  case nvarpar:
    case nfield:
   markdecl(xp->U.V14.tidl);
   if (xp->U.V14.tbind->tt != nid)
    markdecl(xp->U.V14.tbind);
   break ;
    case nscalar:
   markdecl(xp->U.V17.tscalid);
   break ;
    case nrecord:
   markdecl(xp->U.V21.tflist);
   markdecl(xp->U.V21.tvlist);
   break ;
    case nvariant:
   markdecl(xp->U.V20.tvrnt);
   break ;
    case nconfarr:
   if (xp->U.V22.tcelem->tt != nid)
    markdecl(xp->U.V22.tcelem);
   break ;
    case narray:
   if (xp->U.V23.taelem->tt != nid)
    markdecl(xp->U.V23.taelem);
   break ;
    case nsetof:  case nfileof:
   if (xp->U.V18.tof->tt != nid)
    markdecl(xp->U.V18.tof);
   break ;
    case nparproc:  case nparfunc:
   markdecl(xp->U.V15.tparid);
   break ;
    case nptr:  case nsubrange:
   break ;
    default:
   Caseerror(Line);
  }
  xp = xp->tnext;
 }
}

 treeptr
movedecl(tp)
 treeptr tp;
{
 register treeptr R192;
 treeptr ip, np;
 symptr sp;
 boolean move;

 if (tp != (struct S64 *)NIL) {
  move = false;
  switch ((int)(tp->tt)) {
    case nconst:  case ntype:
   ip = tp->U.V14.tidl;
   break ;
    default:
   Caseerror(Line);
  }
  while (ip != (struct S64 *)NIL) {
   if (ip->U.V43.tsym->U.V6.lused) {
    move = true;
    sp = ip->U.V43.tsym;
    if (sp->U.V6.lid->inref > 1)
     sp->U.V6.lid = mkrename('M', sp->U.V6.lid);
    ip = (struct S64 *)NIL;
   } else
    ip = ip->tnext;
  }
  if (move) {
   np = tp->tnext;
   tp->tnext = (struct S64 *)NIL;
   ip = tp;
   while (ip->tt != npgm)
    ip = ip->tup;
   tp->tup = ip;
   switch ((int)(tp->tt)) {
     case nconst:
    if (ip->U.V13.tsubconst == (struct S64 *)NIL)
     ip->U.V13.tsubconst = tp;
    else {
     ip = ip->U.V13.tsubconst;
     while (ip->tnext != (struct S64 *)NIL)
      ip = ip->tnext;
     ip->tnext = tp;
    }
    break ;
     case ntype:
    if (ip->U.V13.tsubtype == (struct S64 *)NIL)
     ip->U.V13.tsubtype = tp;
    else {
     ip = ip->U.V13.tsubtype;
     while (ip->tnext != (struct S64 *)NIL)
      ip = ip->tnext;
     ip->tnext = tp;
    }
    break ;
     default:
    Caseerror(Line);
   }
   tp = movedecl(np);
  } else
   tp->tnext = movedecl(tp->tnext);
 }
 R192 = tp;
 return R192;
}

void movevars();

 void
moveglob(tp, dp)
 treeptr tp, dp;
{
 while (tp->tt != npgm)
  tp = tp->tup;
 dp->tup = tp;
 dp->tnext = tp->U.V13.tsubvar;
 tp->U.V13.tsubvar = dp;
}

 treeptr
stackop(decl, glob, loc)
 treeptr decl, glob, loc;
{
 register treeptr R193;
 treeptr op, ip, dp, tp;

 ip = newid(mkvariable('F'));
 switch ((int)((*G215_vp)->tt)) {
   case nvarpar:  case nvalpar:  case nvar:
  dp = mknode(nvarpar);
  dp->U.V14.tattr = areference;
  dp->U.V14.tidl = ip;
  dp->U.V14.tbind = decl->U.V14.tbind;
  break ;
   case nparproc:  case nparfunc:
  dp = mknode((*G215_vp)->tt);
  dp->U.V15.tparid = ip;
  dp->U.V15.tparparm = (struct S64 *)NIL;
  dp->U.V15.tpartyp = (*G215_vp)->U.V15.tpartyp;
  break ;
   default:
  Caseerror(Line);
 }
 ip->tup = dp;
 tp = decl;
 while (!(Member((unsigned)(tp->tt), Conset[155])))
  tp = tp->tup;
 dp->tup = tp;
 if (tp->U.V13.tsubvar == (struct S64 *)NIL)
  tp->U.V13.tsubvar = dp;
 else {
  tp = tp->U.V13.tsubvar;
  while (tp->tnext != (struct S64 *)NIL)
   tp = tp->tnext;
  tp->tnext = dp;
 }
 dp->tnext = (struct S64 *)NIL;
 op = mknode(npush);
 op->U.V28.tglob = glob;
 op->U.V28.tloc = loc;
 op->U.V28.ttmp = ip;
 R193 = op;
 return R193;
}

 void
addcode(tp, push)
 treeptr tp, push;
{
 treeptr pop;

 pop = mknode(npop);
 pop->U.V28.tglob = push->U.V28.tglob;
 pop->U.V28.ttmp = push->U.V28.ttmp;
 pop->U.V28.tloc = (struct S64 *)NIL;
 push->tnext = tp->U.V13.tsubstmt;
 tp->U.V13.tsubstmt = push;
 push->tup = tp;
 while (push->tnext != (struct S64 *)NIL)
  push = push->tnext;
 push->tnext = pop;
 pop->tup = tp;
}

 void
movevars(tp, vp)
 treeptr tp, vp;
{
 treeptr ep, dp, np;
 idptr ip;
 symptr sp;
 treeptr *F216;

 F216 = G215_vp;
 G215_vp = &vp;
 while ((*G215_vp) != (struct S64 *)NIL) {
  switch ((int)((*G215_vp)->tt)) {
    case nvar:  case nvalpar:  case nvarpar:
   dp = (*G215_vp)->U.V14.tidl;
   break ;
    case nparproc:  case nparfunc:
   dp = (*G215_vp)->U.V15.tparid;
   if (dp->U.V43.tsym->U.V6.lused) {
    ep = mknode((*G215_vp)->tt);
    ep->U.V15.tparparm = (struct S64 *)NIL;
    ep->U.V15.tpartyp = (*G215_vp)->U.V15.tpartyp;
    np = newid(mkrename('G', dp->U.V43.tsym->U.V6.lid));
    ep->U.V15.tparid = np;
    np->tup = ep;
    sp = np->U.V43.tsym;
    ip = sp->U.V6.lid;
    np->U.V43.tsym->U.V6.lid = dp->U.V43.tsym->U.V6.lid;
    dp->U.V43.tsym->U.V6.lid = ip;
    np->U.V43.tsym = dp->U.V43.tsym;
    dp->U.V43.tsym = sp;
    np->U.V43.tsym->lsymdecl = np;
    dp->U.V43.tsym->lsymdecl = dp;
    moveglob(tp, ep);
    addcode(tp, stackop((*G215_vp), np, dp));
   }
   goto L555;
   break ;
    default:
   Caseerror(Line);
  }
  while (dp != (struct S64 *)NIL) {
   if (dp->U.V43.tsym->U.V6.lused) {
    ep = mknode(nvarpar);
    ep->U.V14.tattr = areference;
    np = newid(mkrename('G', dp->U.V43.tsym->U.V6.lid));
    ep->U.V14.tidl = np;
    np->tup = ep;
    ep->U.V14.tbind = (*G215_vp)->U.V14.tbind;
    if (ep->U.V14.tbind->tt == nid)
     ep->U.V14.tbind->U.V43.tsym->U.V6.lused = true;
    sp = np->U.V43.tsym;
    ip = sp->U.V6.lid;
    np->U.V43.tsym->U.V6.lid = dp->U.V43.tsym->U.V6.lid;
    dp->U.V43.tsym->U.V6.lid = ip;
    np->U.V43.tsym = dp->U.V43.tsym;
    dp->U.V43.tsym = sp;
    np->U.V43.tsym->lsymdecl = np;
    dp->U.V43.tsym->lsymdecl = dp;
    dp->tup->U.V14.tattr = aextern;
    moveglob(tp, ep);
    addcode(tp, stackop((*G215_vp), np, dp));
   }
   dp = dp->tnext;
  }
 L555:
  (*G215_vp) = (*G215_vp)->tnext;
 }
 G215_vp = F216;
}

 void
registervar(tp)
 treeptr tp;
{
 treeptr vp, xp;

 vp = idup(tp);
 tp = tp->U.V43.tsym->lsymdecl;
 if ((vp->U.V14.tidl != tp) || (tp->tnext != (struct S64 *)NIL)) {
  xp = mknode(nvar);
  xp->U.V14.tattr = anone;
  xp->U.V14.tidl = tp;
  tp->tup = xp;
  xp->tup = vp->tup;
  xp->U.V14.tbind = vp->U.V14.tbind;
  xp->tnext = vp->tnext;
  vp->tnext = xp;
  if (vp->U.V14.tidl == tp)
   vp->U.V14.tidl = tp->tnext;
  else {
   vp = vp->U.V14.tidl;
   while (vp->tnext != tp)
    vp = vp->tnext;
   vp->tnext = tp->tnext;
  }
  tp->tnext = (struct S64 *)NIL;
 }
 if (tp->tup->U.V14.tattr == anone)
  tp->tup->U.V14.tattr = aregister;
}

 void
cklevel(tp)
 treeptr tp;
{
 tp = tp->U.V43.tsym->lsymdecl;
 while (!(Member((unsigned)(tp->tt), Conset[156])))
  tp = tp->tup;
 if (tp->U.V13.tstat > maxlevel)
  maxlevel = tp->U.V13.tstat;
}

 void
global(tp, dp, depend)
 treeptr tp, dp;
 boolean depend;
{
 treeptr ip;
 boolean dep;

 while (tp != (struct S64 *)NIL) {
  switch ((int)(tp->tt)) {
    case nproc:  case nfunc:
   markdecl(tp->U.V13.tsubid);
   markdecl(tp->U.V13.tsubpar);
   markdecl(tp->U.V13.tsubconst);
   markdecl(tp->U.V13.tsubtype);
   markdecl(tp->U.V13.tsubvar);
   global(tp->U.V13.tsubsub, tp, false);
   global(tp->U.V13.tsubvar, tp, false);
   global(tp->U.V13.tsubtype, tp, false);
   movevars(tp, tp->U.V13.tsubpar);
   movevars(tp, tp->U.V13.tsubvar);
   tp->U.V13.tsubtype = movedecl(tp->U.V13.tsubtype);
   tp->U.V13.tsubconst = movedecl(tp->U.V13.tsubconst);
   global(tp->U.V13.tsubstmt, tp, true);
   global(tp->U.V13.tsubpar, tp, false);
   global(tp->U.V13.tsubvar, tp, false);
   global(tp->U.V13.tsubtype, tp, false);
   global(tp->U.V13.tfuntyp, tp, false);
   break ;
    case npgm:
   markdecl(tp->U.V13.tsubconst);
   markdecl(tp->U.V13.tsubtype);
   markdecl(tp->U.V13.tsubvar);
   global(tp->U.V13.tsubsub, tp, false);
   global(tp->U.V13.tsubstmt, tp, true);
   break ;
    case nconst:  case ntype:  case nvar:  case nfield:
    case nvalpar:  case nvarpar:
   ip = tp->U.V14.tidl;
   dep = depend;
   while ((ip != (struct S64 *)NIL) && !dep) {
    if (ip->U.V43.tsym->U.V6.lused)
     dep = true;
    ip = ip->tnext;
   }
   global(tp->U.V14.tbind, dp, dep);
   break ;
    case nparproc:  case nparfunc:
   global(tp->U.V15.tparparm, dp, depend);
   global(tp->U.V15.tpartyp, dp, depend);
   break ;
    case nsubrange:
   global(tp->U.V19.tlo, dp, depend);
   global(tp->U.V19.thi, dp, depend);
   break ;
    case nvariant:
   global(tp->U.V20.tselct, dp, depend);
   global(tp->U.V20.tvrnt, dp, depend);
   break ;
    case nrecord:
   global(tp->U.V21.tflist, dp, depend);
   global(tp->U.V21.tvlist, dp, depend);
   break ;
    case nconfarr:
   global(tp->U.V22.tcindx, dp, depend);
   global(tp->U.V22.tcelem, dp, depend);
   break ;
    case narray:
   global(tp->U.V23.taindx, dp, depend);
   global(tp->U.V23.taelem, dp, depend);
   break ;
    case nfileof:  case nsetof:
   global(tp->U.V18.tof, dp, depend);
   break ;
    case nptr:
   global(tp->U.V16.tptrid, dp, depend);
   break ;
    case nscalar:
   global(tp->U.V17.tscalid, dp, depend);
   break ;
    case nbegin:
   global(tp->U.V24.tbegin, dp, depend);
   break ;
    case nif:
   global(tp->U.V31.tifxp, dp, depend);
   global(tp->U.V31.tthen, dp, depend);
   global(tp->U.V31.telse, dp, depend);
   break ;
    case nwhile:
   global(tp->U.V32.twhixp, dp, depend);
   global(tp->U.V32.twhistmt, dp, depend);
   break ;
    case nrepeat:
   global(tp->U.V33.treptstmt, dp, depend);
   global(tp->U.V33.treptxp, dp, depend);
   break ;
    case nfor:
   ip = idup(tp->U.V34.tforid);
   if (Member((unsigned)(ip->tup->tt), Conset[157]))
    registervar(tp->U.V34.tforid);
   global(tp->U.V34.tforid, dp, depend);
   global(tp->U.V34.tfrom, dp, depend);
   global(tp->U.V34.tto, dp, depend);
   global(tp->U.V34.tforstmt, dp, depend);
   break ;
    case ncase:
   global(tp->U.V35.tcasxp, dp, depend);
   global(tp->U.V35.tcaslst, dp, depend);
   global(tp->U.V35.tcasother, dp, depend);
   break ;
    case nchoise:
   global(tp->U.V36.tchocon, dp, depend);
   global(tp->U.V36.tchostmt, dp, depend);
   break ;
    case nwith:
   global(tp->U.V37.twithvar, dp, depend);
   global(tp->U.V37.twithstmt, dp, depend);
   break ;
    case nwithvar:
   ip = typeof(tp->U.V38.texpw);
   if (ip->U.V21.tuid == (struct S62 *)NIL)
    ip->U.V21.tuid = mkvariable('S');
   global(tp->U.V38.texpw, dp, depend);
   break ;
    case nlabstmt:
   global(tp->U.V25.tstmt, dp, depend);
   break ;
    case neq:  case nne:  case nlt:  case nle:
    case ngt:  case nge:
   global(tp->U.V41.texpl, dp, depend);
   global(tp->U.V41.texpr, dp, depend);
   ip = typeof(tp->U.V41.texpl);
   if ((ip == typnods.A[(int)(tstring)]) || (ip->tt == narray))
    usecomp = true;
   ip = typeof(tp->U.V41.texpr);
   if ((ip == typnods.A[(int)(tstring)]) || (ip->tt == narray))
    usecomp = true;
   break ;
    case nin:  case nor:  case nplus:  case nminus:
    case nand:  case nmul:  case ndiv:  case nmod:
    case nquot:  case nformat:  case nrange:
   global(tp->U.V41.texpl, dp, depend);
   global(tp->U.V41.texpr, dp, depend);
   break ;
    case nassign:
   global(tp->U.V27.tlhs, dp, depend);
   global(tp->U.V27.trhs, dp, depend);
   break ;
    case nnot:  case numinus:  case nuplus:  case nderef:
   global(tp->U.V42.texps, dp, depend);
   break ;
    case nset:
   global(tp->U.V42.texps, dp, depend);
   break ;
    case nindex:
   global(tp->U.V39.tvariable, dp, depend);
   global(tp->U.V39.toffset, dp, depend);
   break ;
    case nselect:
   global(tp->U.V40.trecord, dp, depend);
   break ;
    case ncall:
   global(tp->U.V30.tcall, dp, depend);
   global(tp->U.V30.taparm, dp, depend);
   break ;
    case nid:
   ip = idup(tp);
   if (ip == (struct S64 *)NIL)
    goto L555;
   do {
    ip = ip->tup;
    if (ip == (struct S64 *)NIL)
     goto L555;
   } while (!(Member((unsigned)(ip->tt), Conset[158])));
   if (dp == ip) {
    if (depend)
     tp->U.V43.tsym->U.V6.lused = true;
   } else {
    tp->U.V43.tsym->U.V6.lused = true;
   }
  L555:
   ;
   break ;
    case ngoto:
   if (!islocal(tp->U.V26.tlabel)) {
    tp->U.V26.tlabel->U.V43.tsym->U.V9.lgo = true;
    usejmps = true;
    cklevel(tp->U.V26.tlabel);
   }
   break ;
    case nbreak:  case npush:  case npop:  case npredef:
    case nempty:  case nchar:  case ninteger:  case nreal:
    case nstring:  case nnil:
   break ;
    default:
   Caseerror(Line);
  }
  tp = tp->tnext;
 }
}

 void
renamc()
{
 idptr ip;
 register cnames cn;

 {
  cnames B49 = cabort,
   B50 = cwrite;

  if ((int)(B49) <= (int)(B50))
   for (cn = B49; ; cn = (cnames)((int)(cn)+1)) {
    ip = mkrename('C', ctable.A[(int)(cn)]);
    ctable.A[(int)(cn)]->istr = ip->istr;
    if (cn == B50) break;
   }
 }
}

 void
renamp(tp, on)
 treeptr tp;
 boolean on;
{
 symptr sp;

 while (tp != (struct S64 *)NIL) {
  renamp(tp->U.V13.tsubsub, true);
  if (on && (tp->U.V13.tsubstmt != (struct S64 *)NIL)) {
   sp = tp->U.V13.tsubid->U.V43.tsym;
   if (sp->U.V6.lid->inref > 1)
    sp->U.V6.lid = mkrename('P', sp->U.V6.lid);
  }
  tp = tp->tnext;
 }
}

void initcode();

 boolean
filevar(tp)
 treeptr tp;
{
 register boolean R194;
 boolean fv;
 treeptr tq;

 switch ((int)(tp->tt)) {
   case npredef:
  fv = (boolean)(tp == typnods.A[(int)(ttext)]);
  break ;
   case nfileof:
  fv = true;
  break ;
   case nconfarr:
  fv = filevar(typeof(tp->U.V22.tcelem));
  break ;
   case narray:
  fv = filevar(typeof(tp->U.V23.taelem));
  break ;
   case nrecord:
  fv = false;
  tq = tp->U.V21.tvlist;
  while (tq != (struct S64 *)NIL) {
   if (filevar(tq->U.V20.tvrnt))
    error(evrntfile);
   tq = tq->tnext;
  }
  tq = tp->U.V21.tflist;
  while (tq != (struct S64 *)NIL) {
   if (filevar(typeof(tq->U.V14.tbind))) {
    fv = true;
    tq = (struct S64 *)NIL;
   } else
    tq = tq->tnext;
  }
  break ;
   case nptr:
  fv = false;
  if (!tp->U.V16.tptrflag) {
   tp->U.V16.tptrflag = true;
   if (filevar(typeof(tp->U.V16.tptrid)))
    error(evarfile);
   tp->U.V16.tptrflag = false;
  }
  break ;
   case nsubrange:  case nscalar:  case nsetof:
  fv = false;
  break ;
   default:
  Caseerror(Line);
 }
 R194 = fv;
 return R194;
}

 treeptr
fileinit(ti, tq, opn)
 treeptr ti, tq;
 boolean opn;
{
 register treeptr R195;
 treeptr tx, ty, tz;

 switch ((int)(tq->tt)) {
   case narray:
  tz = newid(mkvariable('I'));
  ty = mknode(nvar);
  ty->U.V14.tattr = aregister;
  ty->U.V14.tidl = tz;
  ty->U.V14.tbind = typeof(tq->U.V23.taindx);
  tz = tq;
  while (!(Member((unsigned)(tz->tt), Conset[159])))
   tz = tz->tup;
  linkup(tz, ty);
  if (tz->U.V13.tsubvar == (struct S64 *)NIL)
   tz->U.V13.tsubvar = ty;
  else {
   tz = tz->U.V13.tsubvar;
   while (tz->tnext != (struct S64 *)NIL)
    tz = tz->tnext;
   tz->tnext = ty;
  }
  ty = ty->U.V14.tidl;
  tz = mknode(nindex);
  tz->U.V39.tvariable = ti;
  tz->U.V39.toffset = ty;
  tz = fileinit(tz, tq->U.V23.taelem, opn);
  tx = mknode(nfor);
  tx->U.V34.tforid = ty;
  ty = typeof(tq->U.V23.taindx);
  if (ty->tt == nsubrange) {
   tx->U.V34.tfrom = ty->U.V19.tlo;
   tx->U.V34.tto = ty->U.V19.thi;
  } else
   if (ty->tt == nscalar) {
    ty = ty->U.V17.tscalid;
    tx->U.V34.tfrom = ty;
    while (ty->tnext != (struct S64 *)NIL)
     ty = ty->tnext;
    tx->U.V34.tto = ty;
   } else
    if (ty == typnods.A[(int)(tchar)]) {
     currsym.st = schar;
     currsym.U.V2.vchr = minchar;
     tx->U.V34.tfrom = mklit();
     currsym.st = schar;
     currsym.U.V2.vchr = maxchar;
     tx->U.V34.tto = mklit();
    } else
     if (ty == typnods.A[(int)(tinteger)]) {
      currsym.st = sinteger;
      currsym.U.V3.vint = -maxint;
      tx->U.V34.tfrom = mklit();
      currsym.st = sinteger;
      currsym.U.V3.vint = maxint;
      tx->U.V34.tto = mklit();
     } else
      fatal(etree);
  tx->U.V34.tforstmt = tz;
  tx->U.V34.tincr = true;
  break ;
   case npredef:  case nfileof:
  if (opn) {
   ty = mknode(nselect);
   ty->U.V40.trecord = ti;
   ty->U.V40.tfield = oldid(defnams.A[(int)(dzinit)]->U.V6.lid, lforward);
   tx = mknode(nassign);
   tx->U.V27.tlhs = ty;
   currsym.st = sinteger;
   currsym.U.V3.vint = 0;
   tx->U.V27.trhs = mklit();
  } else {
   tx = mknode(ncall);
   tx->U.V30.tcall = oldid(defnams.A[(int)(dclose)]->U.V6.lid, lidentifier);
   tx->U.V30.taparm = ti;
  }
  break ;
   case nrecord:
  ty = (struct S64 *)NIL;
  tq = tq->U.V21.tflist;
  while (tq != (struct S64 *)NIL) {
   if (filevar(typeof(tq->U.V14.tbind))) {
    tz = tq->U.V14.tidl;
    while (tz != (struct S64 *)NIL) {
     tx = mknode(nselect);
     tx->U.V40.trecord = ti;
     tx->U.V40.tfield = tz;
     tx = fileinit(tx, typeof(tq->U.V14.tbind), opn);
     tx->tnext = ty;
     ty = tx;
     tz = tz->tnext;
    }
   }
   tq = tq->tnext;
  }
  tx = mknode(nbegin);
  tx->U.V24.tbegin = ty;
  break ;
   default:
  Caseerror(Line);
 }
 R195 = tx;
 return R195;
}

 void
initcode(tp)
 treeptr tp;
{
 treeptr ti, tq, tu, tv;

 while (tp != (struct S64 *)NIL) {
  initcode(tp->U.V13.tsubsub);
  tv = tp->U.V13.tsubvar;
  while (tv != (struct S64 *)NIL) {
   tq = typeof(tv->U.V14.tbind);
   if (filevar(tq)) {
    ti = tv->U.V14.tidl;
    while (ti != (struct S64 *)NIL) {
     tu = fileinit(ti, tq, true);
     linkup(tp, tu);
     tu->tnext = tp->U.V13.tsubstmt;
     tp->U.V13.tsubstmt = tu;
     while (tu->tnext != (struct S64 *)NIL)
      tu = tu->tnext;
     tu->tnext = fileinit(ti, tq, false);
     linkup(tp, tu->tnext);
     ti = ti->tnext;
    }
   }
   tv = tv->tnext;
  }
  tp = tp->tnext;
 }
}

 void
transform()
{
 renamc();
 renamp(top->U.V13.tsubsub, false);
 extract(top);
 renamf(top);
 initcode(top->U.V13.tsubsub);
 global(top, top, false);
}

void emit();

 void
increment()
{
 (*G230_indnt) = (*G230_indnt) + indstep;
}

 void
decrement()
{
 (*G230_indnt) = (*G230_indnt) - indstep;
}

 void
indent()
{
 integer i;

 i = (*G230_indnt);
 if (i > 60)
  i = i / tabwidth * tabwidth;
 while (i >= tabwidth) {
  Putchr(tab1, output);
  i = i - tabwidth;
 }
 while (i > 0) {
  Putchr(space, output);
  i = i - 1;
 }
}

 boolean
arithexpr(tp)
 treeptr tp;
{
 register boolean R196;

 tp = typeof(tp);
 if (tp->tt == nsubrange)
  if (tp->tup->tt == nconfarr)
   tp = typeof(tp->tup->U.V22.tindtyp);
  else
   tp = typeof(tp->U.V19.tlo);
 R196 = (boolean)((tp == typnods.A[(int)(tinteger)]) || (tp == typnods.A[(int)(tchar)]) || (tp == typnods.A[(int)(treal)]));
 return R196;
}

void eexpr();

void etypedef();

 void
eselect(tp)
 treeptr tp;
{
 boolean da;

 da = (*G226_doarrow);
 (*G226_doarrow) = true;
 eexpr(tp);
 if ((*G228_donearr))
  (*G228_donearr) = false;
 else
  Putchr('.', output);
 (*G226_doarrow) = da;
}

void epredef();

 char
typeletter(tp)
 treeptr tp;
{
 register char R197;
 treeptr tq;

 tq = tp;
 if (tq->tt == nformat) {
  if (tq->U.V41.texpl->tt == nformat) {
   R197 = 'f';
   goto L999;
  }
  tq = tp->U.V41.texpl;
 }
 tq = typeof(tq);
 if (tq->tt == nsubrange)
  tq = typeof(tq->U.V19.tlo);
 if (tq == typnods.A[(int)(tstring)])
  R197 = 's';
 else
  if (tq == typnods.A[(int)(tinteger)])
   R197 = 'd';
  else
   if (tq == typnods.A[(int)(tchar)])
    R197 = 'c';
   else
    if (tq == typnods.A[(int)(treal)])
     if (tp->tt == nformat)
      R197 = 'e';
     else
      R197 = 'g';
    else
     if (tq == typnods.A[(int)(tboolean)]) {
      R197 = 'b';
      (*G217_nelems) = 6;
     } else
      if (tq->tt == narray) {
       R197 = 'a';
       (*G217_nelems) = crange(tq->U.V23.taindx);
      } else
       if (tq->tt == nconfarr) {
        R197 = 'v';
        (*G217_nelems) = 0;
       } else
        fatal(etree);
L999:
 ;
 return R197;
}

 void
etxt(tp)
 treeptr tp;
{
 toknbuf w;
 char c;
 toknidx i;

 switch ((int)(tp->tt)) {
   case nid:
  tp = idup(tp);
  if (tp->tt == nconst)
   etxt(tp->U.V14.tbind);
  else
   fatal(etree);
  break ;
   case nstring:
  gettokn(tp->U.V43.tsym->U.V7.lstr, &w);
  i = 1;
  while (w.A[i - 1] != null) {
   c = w.A[i - 1];
   if ((c == cite) || (c == bslash))
    Putchr(bslash, output);
   else
    if (c == percent)
     Putchr(percent, output);
   Putchr(c, output);
   i = i + 1;
  }
  break ;
   case nchar:
  c = tp->U.V43.tsym->U.V11.lchar;
  if ((c == cite) || (c == bslash))
   Putchr(bslash, output);
  else
   if (c == percent)
    Putchr(percent, output);
  Putchr(c, output);
  break ;
   default:
  Caseerror(Line);
 }
}

 void
eformat(tq)
 treeptr tq;
{
 treeptr tx;
 integer i;

 switch ((int)(typeletter(tq))) {
   case 'a':
  Putchr(percent, output);
  if (tq->tt == nformat)
   if (tq->U.V41.texpr->tt == ninteger)
    eexpr(tq->U.V41.texpr);
   else
    Putchr('*', output);
  (void)fprintf(output.fp, ".%1ds", (*G217_nelems)), Putl(output, 0);
  break ;
   case 'b':
  Putchr(percent, output);
  if (tq->tt == nformat) {
   if (tq->U.V41.texpr->tt == ninteger)
    eexpr(tq->U.V41.texpr);
   else
    Putchr('*', output);
  }
  Putchr('s', output);
  break ;
   case 'c':
  if (tq->tt == nchar)
   etxt(tq);
  else {
   Putchr(percent, output);
   if (tq->tt == nformat)
    if (tq->U.V41.texpr->tt == ninteger)
     eexpr(tq->U.V41.texpr);
    else
     Putchr('*', output);
   Putchr('c', output);
  }
  break ;
   case 'd':
  Putchr(percent, output);
  if (tq->tt == nformat) {
   if (tq->U.V41.texpr->tt == ninteger)
    eexpr(tq->U.V41.texpr);
   else
    Putchr('*', output);
  } else
   (void)fprintf(output.fp, "%1d", intlen), Putl(output, 0);
  Putchr('d', output);
  break ;
   case 'e':
  (void)fprintf(output.fp, "%c%c", percent, space), Putl(output, 0);
  tx = tq->U.V41.texpr;
  if (tx->tt == ninteger) {
   i = cvalof(tx);
   (void)fprintf(output.fp, "%1d.", i), Putl(output, 0);
   i = i - 7;
   if (i < 1)
    Putchr('1', output);
   else
    (void)fprintf(output.fp, "%1d", i), Putl(output, 0);
  } else
   (void)fprintf(output.fp, "*.*"), Putl(output, 0);
  Putchr('e', output);
  break ;
   case 'f':
  Putchr(percent, output);
  tx = tq->U.V41.texpl;
  if (tx->U.V41.texpr->tt == ninteger) {
   eexpr(tx->U.V41.texpr);
   Putchr('.', output);
   tx = tq->U.V41.texpr;
   if (tx->tt == ninteger) {
    i = cvalof(tx);
    tx = tq->U.V41.texpl->U.V41.texpr;
    if (i > cvalof(tx) - 1)
     Putchr('1', output);
    else
     (void)fprintf(output.fp, "%1d", i), Putl(output, 0);
   } else
    Putchr('*', output);
  } else
   (void)fprintf(output.fp, "*.*"), Putl(output, 0);
  Putchr('f', output);
  break ;
   case 'g':
  (void)fprintf(output.fp, "%c%1de", percent, fixlen), Putl(output, 0);
  break ;
   case 's':
  if (tq->tt == nstring)
   etxt(tq);
  else {
   Putchr(percent, output);
   if (tq->tt == nformat)
    if (tq->U.V41.texpr->tt == ninteger)
     eexpr(tq->U.V41.texpr);
    else
     (void)fprintf(output.fp, "*.*"), Putl(output, 0);
   Putchr('s', output);
  }
  break ;
   case 'v':
  fatal(eprconf);
  break ;
   default:
  Caseerror(Line);
 }
}

 void
ewrite(tq)
 treeptr tq;
{
 treeptr tx;

 switch ((int)(typeletter(tq))) {
   case 'a':
  (void)fprintf(output.fp, ", "), Putl(output, 0);
  tx = tq;
  if (tq->tt == nformat) {
   if (tq->U.V41.texpr->tt != ninteger) {
    eexpr(tq->U.V41.texpr);
    (void)fprintf(output.fp, ", "), Putl(output, 0);
   }
   tx = tq->U.V41.texpl;
  }
  eexpr(tx);
  (void)fprintf(output.fp, ".A"), Putl(output, 0);
  break ;
   case 'b':
  (void)fprintf(output.fp, ", "), Putl(output, 0);
  tx = tq;
  if (tq->tt == nformat) {
   if (tq->U.V41.texpr->tt != ninteger) {
    eexpr(tq->U.V41.texpr);
    (void)fprintf(output.fp, ", "), Putl(output, 0);
   }
   tx = tq->U.V41.texpl;
  }
  usebool = true;
  (void)fprintf(output.fp, "Bools[(int)("), Putl(output, 0);
  eexpr(tx);
  (void)fprintf(output.fp, ")]"), Putl(output, 0);
  break ;
   case 'c':
  if (tq->tt == nformat) {
   if (tq->U.V41.texpr->tt != ninteger) {
    (void)fprintf(output.fp, ", "), Putl(output, 0);
    eexpr(tq->U.V41.texpr);
   }
   (void)fprintf(output.fp, ", "), Putl(output, 0);
   eexpr(tq->U.V41.texpl);
  } else
   if (tq->tt != nchar) {
    (void)fprintf(output.fp, ", "), Putl(output, 0);
    eexpr(tq);
   }
  break ;
   case 'd':
  (void)fprintf(output.fp, ", "), Putl(output, 0);
  tx = tq;
  if (tq->tt == nformat) {
   if (tq->U.V41.texpr->tt != ninteger) {
    eexpr(tq->U.V41.texpr);
    (void)fprintf(output.fp, ", "), Putl(output, 0);
   }
   tx = tq->U.V41.texpl;
  }
  eexpr(tx);
  break ;
   case 'e':
  (void)fprintf(output.fp, ", "), Putl(output, 0);
  tx = tq->U.V41.texpr;
  if (tx->tt != ninteger) {
   usemax = true;
   eexpr(tx);
   (void)fprintf(output.fp, ", Max("), Putl(output, 0);
   eexpr(tx);
   (void)fprintf(output.fp, " - 7, 1), "), Putl(output, 0);
  }
  eexpr(tq->U.V41.texpl);
  break ;
   case 'f':
  (void)fprintf(output.fp, ", "), Putl(output, 0);
  tx = tq->U.V41.texpl;
  if (tx->U.V41.texpr->tt != ninteger) {
   eexpr(tx->U.V41.texpr);
   (void)fprintf(output.fp, ", "), Putl(output, 0);
  }
  if ((tx->U.V41.texpr->tt != ninteger) || (tq->U.V41.texpr->tt != ninteger)) {
   usemax = true;
   (void)fprintf(output.fp, "Max(("), Putl(output, 0);
   eexpr(tx->U.V41.texpr);
   (void)fprintf(output.fp, ") - ("), Putl(output, 0);
   eexpr(tq->U.V41.texpr);
   (void)fprintf(output.fp, ") - 1, 1), "), Putl(output, 0);
  }
  eexpr(tq->U.V41.texpl->U.V41.texpl);
  break ;
   case 'g':
  (void)fprintf(output.fp, ", "), Putl(output, 0);
  eexpr(tq);
  break ;
   case 's':
  if (tq->tt == nformat) {
   if (tq->U.V41.texpr->tt != ninteger) {
    (void)fprintf(output.fp, ", "), Putl(output, 0);
    eexpr(tq->U.V41.texpr);
    (void)fprintf(output.fp, ", "), Putl(output, 0);
    eexpr(tq->U.V41.texpr);
   }
   (void)fprintf(output.fp, ", "), Putl(output, 0);
   eexpr(tq->U.V41.texpl);
  } else
   if (tq->tt != nstring) {
    (void)fprintf(output.fp, ", "), Putl(output, 0);
    eexpr(tq);
   }
  break ;
   case 'v':
  fatal(eprconf);
  break ;
   default:
  Caseerror(Line);
 }
}

void enewsize();

 void
esubsize(tp, tq)
 treeptr tp, tq;
{
 treeptr tx, ty;
 boolean addsize;

 tx = tq->U.V20.tvrnt;
 ty = tx->U.V21.tflist;
 if (ty == (struct S64 *)NIL) {
  ty = tx->U.V21.tvlist;
  while (ty != (struct S64 *)NIL) {
   if (ty->U.V20.tvrnt->U.V21.tflist != (struct S64 *)NIL) {
    ty = ty->U.V20.tvrnt->U.V21.tflist;
    goto L555;
   }
   ty = ty->tnext;
  }
 L555:
  ;
 }
 addsize = true;
 if (ty == (struct S64 *)NIL) {
  addsize = false;
  ty = tx->tup->tup->U.V21.tvlist;
  while (ty != (struct S64 *)NIL) {
   if (ty->U.V20.tvrnt->U.V21.tflist != (struct S64 *)NIL) {
    ty = ty->U.V20.tvrnt->U.V21.tflist;
    goto L666;
   }
   ty = ty->tnext;
  }
 L666:
  ;
 }
 if (ty == (struct S64 *)NIL) {
  (void)fprintf(output.fp, "sizeof(*"), Putl(output, 0);
  eexpr(tp);
  Putchr(')', output);
 } else {
  (void)fprintf(output.fp, "Unionoffs("), Putl(output, 0);
  eexpr(tp);
  (void)fprintf(output.fp, ", "), Putl(output, 0);
  printid(ty->U.V14.tidl->U.V43.tsym->U.V6.lid);
  if (addsize) {
   (void)fprintf(output.fp, ") + sizeof("), Putl(output, 0);
   eexpr(tp);
   (void)fprintf(output.fp, "->"), Putl(output, 0);
   printid(tx->U.V21.tuid);
  }
  Putchr(')', output);
 }
}

 void
enewsize(tp)
 treeptr tp;
{
 treeptr tq, tx, ty;
 integer v;

 if ((tp->tnext != (struct S64 *)NIL) && unionnew) {
  v = cvalof(tp->tnext);
  tq = typeof(tp);
  tq = typeof(tq->U.V16.tptrid);
  if (tq->tt != nrecord)
   fatal(etree);
  tx = tq->U.V21.tvlist;
  while (tx != (struct S64 *)NIL) {
   ty = tx->U.V20.tselct;
   while (ty != (struct S64 *)NIL) {
    if (v == cvalof(ty))
     goto L555;
    ty = ty->tnext;
   }
   tx = tx->tnext;
  }
  fatal(etag);
 L555:
  esubsize(tp, tx);
 } else {
  (void)fprintf(output.fp, "sizeof(*"), Putl(output, 0);
  eexpr(tp);
  Putchr(')', output);
 }
}

 void
epredef(ts, tp)
 treeptr ts, tp;
{
 treeptr tq, tv, tx;
 predefs td;
 integer nelems;
 char ch;
 boolean txtfile;
 integer *F218;

 F218 = G217_nelems;
 G217_nelems = &nelems;
 td = ts->U.V13.tsubstmt->U.V12.tdef;
 switch ((int)(td)) {
   case dabs:
  tq = typeof(tp->U.V30.taparm);
  if ((tq == typnods.A[(int)(tinteger)]) || (tq->tt == nsubrange))
   (void)fprintf(output.fp, "abs("), Putl(output, 0);
  else
   (void)fprintf(output.fp, "fabs("), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  Putchr(')', output);
  break ;
   case dargv:
  (void)fprintf(output.fp, "Argvgt("), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  (void)fprintf(output.fp, ", "), Putl(output, 0);
  eexpr(tp->U.V30.taparm->tnext);
  (void)fprintf(output.fp, ".A, sizeof("), Putl(output, 0);
  eexpr(tp->U.V30.taparm->tnext);
  (void)fprintf(output.fp, ".A));\n"), Putl(output, 1);
  break ;
   case dchr:
  tq = typeof(tp->U.V30.taparm);
  if (tq->tt == nsubrange)
   if (tq->tup->tt == nconfarr)
    tq = typeof(tq->tup->U.V22.tindtyp);
   else
    tq = typeof(tq->U.V19.tlo);
  if ((tq == typnods.A[(int)(tinteger)]) || (tq == typnods.A[(int)(tchar)]))
   eexpr(tp->U.V30.taparm);
  else {
   (void)fprintf(output.fp, "(char)("), Putl(output, 0);
   eexpr(tp->U.V30.taparm);
   Putchr(')', output);
  }
  break ;
   case ddispose:
  (void)fprintf(output.fp, "free("), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  break ;
   case deof:
  (void)fprintf(output.fp, "Eof("), Putl(output, 0);
  if (tp->U.V30.taparm == (struct S64 *)NIL) {
   defnams.A[(int)(dinput)]->U.V6.lused = true;
   printid(defnams.A[(int)(dinput)]->U.V6.lid);
  } else
   eexpr(tp->U.V30.taparm);
  Putchr(')', output);
  break ;
   case deoln:
  (void)fprintf(output.fp, "Eoln("), Putl(output, 0);
  if (tp->U.V30.taparm == (struct S64 *)NIL) {
   defnams.A[(int)(dinput)]->U.V6.lused = true;
   printid(defnams.A[(int)(dinput)]->U.V6.lid);
  } else
   eexpr(tp->U.V30.taparm);
  Putchr(')', output);
  break ;
   case dexit:
  (void)fprintf(output.fp, "exit("), Putl(output, 0);
  if (tp->U.V30.taparm == (struct S64 *)NIL)
   Putchr('0', output);
  else
   eexpr(tp->U.V30.taparm);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  break ;
   case dflush:
  (void)fprintf(output.fp, "fflush("), Putl(output, 0);
  if (tp->U.V30.taparm == (struct S64 *)NIL) {
   defnams.A[(int)(doutput)]->U.V6.lused = true;
   printid(defnams.A[(int)(doutput)]->U.V6.lid);
  } else
   eexpr(tp->U.V30.taparm);
  (void)fprintf(output.fp, ".fp);\n"), Putl(output, 1);
  break ;
   case dpage:
  (void)fprintf(output.fp, "Putchr(%s, ", ffchr), Putl(output, 0);
  if (tp->U.V30.taparm == (struct S64 *)NIL) {
   defnams.A[(int)(doutput)]->U.V6.lused = true;
   printid(defnams.A[(int)(doutput)]->U.V6.lid);
  } else
   eexpr(tp->U.V30.taparm);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  break ;
   case dput:  case dget:
  if (typeof(tp->U.V30.taparm) == typnods.A[(int)(ttext)])
   if (td == dget)
    (void)fprintf(output.fp, "Getx"), Putl(output, 0);
   else
    (void)fprintf(output.fp, "Putx"), Putl(output, 0);
  else {
   (void)fprintf(output.fp, "%s", voidcast), Putl(output, 0);
   if (td == dget)
    (void)fprintf(output.fp, "Get"), Putl(output, 0);
   else
    (void)fprintf(output.fp, "Put"), Putl(output, 0);
  }
  Putchr('(', output);
  eexpr(tp->U.V30.taparm);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  break ;
   case dhalt:
  (void)fprintf(output.fp, "abort();\n"), Putl(output, 1);
  break ;
   case dnew:
  eexpr(tp->U.V30.taparm);
  (void)fprintf(output.fp, " = ("), Putl(output, 0);
  etypedef(typeof(tp->U.V30.taparm));
  (void)fprintf(output.fp, ")malloc((unsigned)("), Putl(output, 0);
  enewsize(tp->U.V30.taparm);
  (void)fprintf(output.fp, "));\n"), Putl(output, 1);
  break ;
   case dord:
  (void)fprintf(output.fp, "(unsigned)("), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  Putchr(')', output);
  break ;
   case dread:  case dreadln:
  txtfile = false;
  tq = tp->U.V30.taparm;
  if (tq != (struct S64 *)NIL) {
   tv = typeof(tq);
   if (tv == typnods.A[(int)(ttext)]) {
    txtfile = true;
    tv = tq;
    tq = tq->tnext;
   } else
    if (tv->tt == nfileof) {
     txtfile = (boolean)(typeof(tv->U.V18.tof) == typnods.A[(int)(tchar)]);
     tv = tq;
     tq = tq->tnext;
    } else {
     txtfile = true;
     tv = (struct S64 *)NIL;
    }
  } else {
   tv = (struct S64 *)NIL;
   txtfile = true;
  }
  if (txtfile) {
   if (tq == (struct S64 *)NIL)
    goto L444;
   if ((tq->tt != nformat) && (tq->tnext == (struct S64 *)NIL) && (typeletter(tq) == 'c')) {
    eexpr(tq);
    (void)fprintf(output.fp, " = "), Putl(output, 0);
    (void)fprintf(output.fp, "Getchr("), Putl(output, 0);
    if (tv == (struct S64 *)NIL)
     printid(defnams.A[(int)(dinput)]->U.V6.lid);
    else
     eexpr(tv);
    Putchr(')', output);
    if (td == dreadln)
     Putchr(',', output);
    goto L444;
   }
   usescan = true;
   (void)fprintf(output.fp, "Fscan("), Putl(output, 0);
   if (tv == (struct S64 *)NIL)
    printid(defnams.A[(int)(dinput)]->U.V6.lid);
   else
    eexpr(tv);
   (void)fprintf(output.fp, "), "), Putl(output, 0);
   while (tq != (struct S64 *)NIL) {
    (void)fprintf(output.fp, "Scan(%c", cite), Putl(output, 0);
    ch = typeletter(tq);
    switch ((int)(ch)) {
      case 'a':
     (void)fprintf(output.fp, "%cs", percent), Putl(output, 0);
     break ;
      case 'c':
     (void)fprintf(output.fp, "%cc", percent), Putl(output, 0);
     break ;
      case 'd':
     (void)fprintf(output.fp, "%cld", percent), Putl(output, 0);
     break ;
      case 'g':
     (void)fprintf(output.fp, "%cle", percent), Putl(output, 0);
     break ;
      default:
     Caseerror(Line);
    }
    (void)fprintf(output.fp, "%c, ", cite), Putl(output, 0);
    switch ((int)(ch)) {
      case 'a':
     eexpr(tq);
     (void)fprintf(output.fp, ".A"), Putl(output, 0);
     break ;
      case 'c':
     Putchr('&', output);
     eexpr(tq);
     break ;
      case 'd':
     (void)fprintf(output.fp, "&Tmplng"), Putl(output, 0);
     break ;
      case 'g':
     (void)fprintf(output.fp, "&Tmpdbl"), Putl(output, 0);
     break ;
      default:
     Caseerror(Line);
    }
    Putchr(')', output);
    switch ((int)(ch)) {
      case 'd':
     (void)fprintf(output.fp, ", "), Putl(output, 0);
     eexpr(tq);
     (void)fprintf(output.fp, " = Tmplng"), Putl(output, 0);
     break ;
      case 'g':
     (void)fprintf(output.fp, ", "), Putl(output, 0);
     eexpr(tq);
     (void)fprintf(output.fp, " = Tmpdbl"), Putl(output, 0);
     break ;
      case 'a':  case 'c':
     break ;
      default:
     Caseerror(Line);
    }
    tq = tq->tnext;
    if (tq != (struct S64 *)NIL) {
     Putchr(',', output),Putchr('\n', output);
     indent();
     Putchr(tab1, output);
    }
   }
   (void)fprintf(output.fp, ", Getx("), Putl(output, 0);
   if (tv == (struct S64 *)NIL)
    printid(defnams.A[(int)(dinput)]->U.V6.lid);
   else
    eexpr(tv);
   Putchr(')', output);
   if (td == dreadln)
    Putchr(',', output);
  L444:
   if (td == dreadln) {
    usegetl = true;
    (void)fprintf(output.fp, "Getl(&"), Putl(output, 0);
    if (tv == (struct S64 *)NIL)
     printid(defnams.A[(int)(dinput)]->U.V6.lid);
    else
     eexpr(tv);
    Putchr(')', output);
   }
  } else {
   increment();
   while (tq != (struct S64 *)NIL) {
    (void)fprintf(output.fp, "%sFread(", voidcast), Putl(output, 0);
    eexpr(tq);
    (void)fprintf(output.fp, ", "), Putl(output, 0);
    eexpr(tv);
    (void)fprintf(output.fp, ".fp)"), Putl(output, 0);
    tq = tq->tnext;
    if (tq != (struct S64 *)NIL) {
     Putchr(',', output),Putchr('\n', output);
     indent();
    }
   }
   decrement();
  }
  Putchr(';', output),Putchr('\n', output);
  break ;
   case dwrite:  case dwriteln:  case dmessage:
  txtfile = false;
  tq = tp->U.V30.taparm;
  if (tq != (struct S64 *)NIL) {
   tv = typeof(tq);
   if (tv == typnods.A[(int)(ttext)]) {
    txtfile = true;
    tv = tq;
    tq = tq->tnext;
   } else
    if (tv->tt == nfileof) {
     txtfile = (boolean)(typeof(tv->U.V18.tof) == typnods.A[(int)(tchar)]);
     tv = tq;
     tq = tq->tnext;
    } else {
     txtfile = true;
     tv = (struct S64 *)NIL;
    }
  } else {
   tv = (struct S64 *)NIL;
   txtfile = true;
  }
  if (txtfile) {
   if (tq == (struct S64 *)NIL) {
    if (Member((unsigned)(td), Conset[160])) {
     (void)fprintf(output.fp, "Putchr(%s, ", nlchr), Putl(output, 0);
     if (tv == (struct S64 *)NIL)
      printid(defnams.A[(int)(doutput)]->U.V6.lid);
     else
      eexpr(tv);
     Putchr(')', output);
    }
    Putchr(';', output),Putchr('\n', output);
    goto L555;
   } else
    if ((tq->tt != nformat) && (tq->tnext == (struct S64 *)NIL))
     if (typeletter(tq) == 'c') {
      (void)fprintf(output.fp, "Putchr("), Putl(output, 0);
      eexpr(tq);
      (void)fprintf(output.fp, ", "), Putl(output, 0);
      if (tv == (struct S64 *)NIL)
       printid(defnams.A[(int)(doutput)]->U.V6.lid);
      else
       eexpr(tv);
      Putchr(')', output);
      if (td == dwriteln) {
       (void)fprintf(output.fp, ",Putchr(%s, ", nlchr), Putl(output, 0);
       if (tv == (struct S64 *)NIL)
        printid(defnams.A[(int)(doutput)]->U.V6.lid);
       else
        eexpr(tv);
       Putchr(')', output);
      }
      Putchr(';', output),Putchr('\n', output);
      goto L555;
     }
   tx = (struct S64 *)NIL;
   (void)fprintf(output.fp, "%sfprintf(", voidcast), Putl(output, 0);
   if (td == dmessage)
    (void)fprintf(output.fp, "stderr, "), Putl(output, 0);
   else {
    if (tv == (struct S64 *)NIL)
     printid(defnams.A[(int)(doutput)]->U.V6.lid);
    else
     eexpr(tv);
    (void)fprintf(output.fp, ".fp, "), Putl(output, 0);
   }
   Putchr(cite, output);
   tx = tq;
   while (tq != (struct S64 *)NIL) {
    eformat(tq);
    tq = tq->tnext;
   }
   if ((td == dmessage) || (td == dwriteln))
    (void)fprintf(output.fp, "\\n"), Putl(output, 0);
   Putchr(cite, output);
   tq = tx;
   while (tq != (struct S64 *)NIL) {
    ewrite(tq);
    tq = tq->tnext;
   }
   (void)fprintf(output.fp, "), Putl("), Putl(output, 0);
   if (tv == (struct S64 *)NIL)
    printid(defnams.A[(int)(doutput)]->U.V6.lid);
   else
    eexpr(tv);
   if (td == dwrite)
    (void)fprintf(output.fp, ", 0)"), Putl(output, 0);
   else
    (void)fprintf(output.fp, ", 1)"), Putl(output, 0);
  } else {
   increment();
   tx = typeof(tv);
   if (tx == typnods.A[(int)(ttext)])
    tx = typnods.A[(int)(tchar)];
   else
    if (tx->tt == nfileof)
     tx = typeof(tx->U.V18.tof);
    else
     fatal(etree);
   while (tq != (struct S64 *)NIL) {
    if ((Member((unsigned)(tq->tt), Conset[161])) && (tx == typeof(tq))) {
     (void)fprintf(output.fp, "%sFwrite(", voidcast), Putl(output, 0);
     eexpr(tq);
    } else {
     if (tx->tt == nsetof) {
      usescpy = true;
      (void)fprintf(output.fp, "Setncpy("), Putl(output, 0);
      eselect(tv);
      (void)fprintf(output.fp, "buf.S, "), Putl(output, 0);
      eexpr(tq);
      if (typeof(tp->U.V27.trhs) == typnods.A[(int)(tset)])
       eexpr(tq);
      else {
       eselect(tq);
       Putchr('S', output);
      }
      (void)fprintf(output.fp, ", sizeof("), Putl(output, 0);
      eexpr(tv);
      (void)fprintf(output.fp, ".buf))"), Putl(output, 0);
     } else {
      eexpr(tv);
      (void)fprintf(output.fp, ".buf = "), Putl(output, 0);
      eexpr(tq);
     }
     (void)fprintf(output.fp, ", Fwrite("), Putl(output, 0);
     eexpr(tv);
     (void)fprintf(output.fp, ".buf"), Putl(output, 0);
    }
    (void)fprintf(output.fp, ", "), Putl(output, 0);
    eexpr(tv);
    (void)fprintf(output.fp, ".fp)"), Putl(output, 0);
    tq = tq->tnext;
    if (tq != (struct S64 *)NIL) {
     Putchr(',', output),Putchr('\n', output);
     indent();
    }
   }
   decrement();
  }
  Putchr(';', output),Putchr('\n', output);
 L555:
  ;
  break ;
   case dclose:
  tq = typeof(tp->U.V30.taparm);
  txtfile = (boolean)(tq == typnods.A[(int)(ttext)]);
  if ((!txtfile) && (tq->tt == nfileof))
   if (typeof(tq->U.V18.tof) == typnods.A[(int)(tchar)])
    txtfile = true;
  if (txtfile)
   (void)fprintf(output.fp, "Closex("), Putl(output, 0);
  else
   (void)fprintf(output.fp, "Close("), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  break ;
   case dreset:  case drewrite:
  tq = typeof(tp->U.V30.taparm);
  txtfile = (boolean)(tq == typnods.A[(int)(ttext)]);
  if ((!txtfile) && (tq->tt == nfileof))
   if (typeof(tq->U.V18.tof) == typnods.A[(int)(tchar)])
    txtfile = true;
  if (txtfile)
   if (td == dreset)
    (void)fprintf(output.fp, "Resetx("), Putl(output, 0);
   else
    (void)fprintf(output.fp, "Rewritex("), Putl(output, 0);
  else
   if (td == dreset)
    (void)fprintf(output.fp, "Reset("), Putl(output, 0);
   else
    (void)fprintf(output.fp, "Rewrite("), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  (void)fprintf(output.fp, ", "), Putl(output, 0);
  tq = tp->U.V30.taparm->tnext;
  if (tq == (struct S64 *)NIL)
   (void)fprintf(output.fp, "NULL, 0"), Putl(output, 0);
  else {
   tq = typeof(tq);
   if (tq == typnods.A[(int)(tchar)]) {
    Putchr(cite, output);
    ch = cvalof(tp->U.V30.taparm->tnext);
    if ((ch == bslash) || (ch == cite))
     Putchr(bslash, output);
    (void)fprintf(output.fp, "%c%c, -1", ch, cite), Putl(output, 0);
   } else
    if (tq == typnods.A[(int)(tstring)]) {
     eexpr(tp->U.V30.taparm->tnext);
     (void)fprintf(output.fp, ", -1"), Putl(output, 0);
    } else
     if (tq->tt == narray) {
      eexpr(tp->U.V30.taparm->tnext);
      (void)fprintf(output.fp, ".A, sizeof("), Putl(output, 0);
      eexpr(tp->U.V30.taparm->tnext);
      (void)fprintf(output.fp, ".A)"), Putl(output, 0);
     } else
      fatal(etree);
  }
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  break ;
   case darctan:
  (void)fprintf(output.fp, "atan("), Putl(output, 0);
  if (typeof(tp->U.V30.taparm) != typnods.A[(int)(treal)])
   (void)fprintf(output.fp, "%s", dblcast), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  Putchr(')', output);
  break ;
   case dln:
  (void)fprintf(output.fp, "log("), Putl(output, 0);
  if (typeof(tp->U.V30.taparm) != typnods.A[(int)(treal)])
   (void)fprintf(output.fp, "%s", dblcast), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  Putchr(')', output);
  break ;
   case dexp:
  (void)fprintf(output.fp, "exp("), Putl(output, 0);
  if (typeof(tp->U.V30.taparm) != typnods.A[(int)(treal)])
   (void)fprintf(output.fp, "%s", dblcast), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  Putchr(')', output);
  break ;
   case dcos:  case dsin:  case dsqrt:
  eexpr(tp->U.V30.tcall);
  Putchr('(', output);
  if (typeof(tp->U.V30.taparm) != typnods.A[(int)(treal)])
   (void)fprintf(output.fp, "%s", dblcast), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  Putchr(')', output);
  break ;
   case dtan:
  (void)fprintf(output.fp, "atan("), Putl(output, 0);
  if (typeof(tp->U.V30.taparm) != typnods.A[(int)(treal)])
   (void)fprintf(output.fp, "%s", dblcast), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  Putchr(')', output);
  break ;
   case dsucc:  case dpred:
  tq = typeof(tp->U.V30.taparm);
  if (tq->tt == nsubrange)
   if (tq->tup->tt == nconfarr)
    tq = typeof(tq->tup->U.V22.tindtyp);
   else
    tq = typeof(tq->U.V19.tlo);
  if ((tq == typnods.A[(int)(tinteger)]) || (tq == typnods.A[(int)(tchar)])) {
   (void)fprintf(output.fp, "(("), Putl(output, 0);
   eexpr(tp->U.V30.taparm);
   if (td == dpred)
    (void)fprintf(output.fp, ")-1)"), Putl(output, 0);
   else
    (void)fprintf(output.fp, ")+1)"), Putl(output, 0);
  } else {
   Putchr('(', output);
   tq = tq->tup;
   if (tq->tt == ntype) {
    Putchr('(', output);
    printid(tq->U.V14.tidl->U.V43.tsym->U.V6.lid);
    Putchr(')', output);
   }
   (void)fprintf(output.fp, "((int)("), Putl(output, 0);
   eexpr(tp->U.V30.taparm);
   if (td == dpred)
    (void)fprintf(output.fp, ")-1))"), Putl(output, 0);
   else
    (void)fprintf(output.fp, ")+1))"), Putl(output, 0);
  }
  break ;
   case dodd:
  Putchr('(', output);
  printid(defnams.A[(int)(dboolean)]->U.V6.lid);
  (void)fprintf(output.fp, ")(("), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  (void)fprintf(output.fp, ") & 1)"), Putl(output, 0);
  break ;
   case dsqr:
  tq = typeof(tp->U.V30.taparm);
  if ((tq == typnods.A[(int)(tinteger)]) || (tq->tt == nsubrange)) {
   (void)fprintf(output.fp, "(("), Putl(output, 0);
   eexpr(tp->U.V30.taparm);
   (void)fprintf(output.fp, ") * ("), Putl(output, 0);
   eexpr(tp->U.V30.taparm);
   (void)fprintf(output.fp, "))"), Putl(output, 0);
  } else {
   (void)fprintf(output.fp, "pow("), Putl(output, 0);
   if (typeof(tp->U.V30.taparm) != typnods.A[(int)(treal)])
    (void)fprintf(output.fp, "%s", dblcast), Putl(output, 0);
   eexpr(tp->U.V30.taparm);
   (void)fprintf(output.fp, ", 2.0)"), Putl(output, 0);
  }
  break ;
   case dround:
  (void)fprintf(output.fp, "Round("), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  Putchr(')', output);
  break ;
   case dtrunc:
  (void)fprintf(output.fp, "Trunc("), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  Putchr(')', output);
  break ;
   case dpack:
  tq = typeof(tp->U.V30.taparm);
  tx = typeof(tp->U.V30.taparm->tnext->tnext);
  (void)fprintf(output.fp, "{    %s%s%c_j, _i = ", registr, inttyp, tab1), Putl(output, 0);
  if (!arithexpr(tp->U.V30.taparm->tnext))
   (void)fprintf(output.fp, "(int)"), Putl(output, 0);
  eexpr(tp->U.V30.taparm->tnext);
  if (tx->tt == narray)
   (void)fprintf(output.fp, " - %1d", clower(tq->U.V23.taindx)), Putl(output, 0);
  Putchr(';', output),Putchr('\n', output);
  indent();
  (void)fprintf(output.fp, "    for (_j = 0; _j < "), Putl(output, 0);
  if (tq->tt == nconfarr) {
   (void)fprintf(output.fp, "(int)("), Putl(output, 0);
   printid(tx->U.V22.tcindx->U.V19.thi->U.V43.tsym->U.V6.lid);
   Putchr(')', output);
  } else
   (void)fprintf(output.fp, "%1d", crange(tx->U.V23.taindx)), Putl(output, 0);
  (void)fprintf(output.fp, "; )\n"), Putl(output, 1);
  indent();
  Putchr(tab1, output);
  eexpr(tp->U.V30.taparm->tnext->tnext);
  (void)fprintf(output.fp, ".A[_j++] = "), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  (void)fprintf(output.fp, ".A[_i++];\n"), Putl(output, 1);
  indent();
  Putchr('}', output),Putchr('\n', output);
  break ;
   case dunpack:
  tq = typeof(tp->U.V30.taparm);
  tx = typeof(tp->U.V30.taparm->tnext);
  (void)fprintf(output.fp, "{   %s%s%c_j, _i = ", registr, inttyp, tab1), Putl(output, 0);
  if (!arithexpr(tp->U.V30.taparm->tnext->tnext))
   (void)fprintf(output.fp, "(int)"), Putl(output, 0);
  eexpr(tp->U.V30.taparm->tnext->tnext);
  if (tx->tt != nconfarr)
   (void)fprintf(output.fp, " - %1d", clower(tx->U.V23.taindx)), Putl(output, 0);
  Putchr(';', output),Putchr('\n', output);
  indent();
  (void)fprintf(output.fp, "    for (_j = 0; _j < "), Putl(output, 0);
  if (tq->tt == nconfarr) {
   (void)fprintf(output.fp, "(int)("), Putl(output, 0);
   printid(tq->U.V22.tcindx->U.V19.thi->U.V43.tsym->U.V6.lid);
   Putchr(')', output);
  } else
   (void)fprintf(output.fp, "%1d", crange(tq->U.V23.taindx)), Putl(output, 0);
  (void)fprintf(output.fp, "; )\n"), Putl(output, 1);
  indent();
  Putchr(tab1, output);
  eexpr(tp->U.V30.taparm->tnext);
  (void)fprintf(output.fp, ".A[_i++] = "), Putl(output, 0);
  eexpr(tp->U.V30.taparm);
  (void)fprintf(output.fp, ".A[_j++];\n"), Putl(output, 1);
  indent();
  Putchr('}', output),Putchr('\n', output);
  break ;
   default:
  Caseerror(Line);
 }
 G217_nelems = F218;
}

 void
eaddr(tp)
 treeptr tp;
{
 Putchr('&', output);
 if (!(Member((unsigned)(tp->tt), Conset[162])))
  error(evarpar);
 eexpr(tp);
}

 void
ecall(tp)
 treeptr tp;
{
 treeptr tf, tq, tx;

 tf = idup(tp->U.V30.tcall);
 switch ((int)(tf->tt)) {
   case nproc:  case nfunc:
  tf = tf->U.V13.tsubpar;
  break ;
   case nparproc:  case nparfunc:
  tf = tf->U.V15.tparparm;
  break ;
   default:
  Caseerror(Line);
 }
 if (tf != (struct S64 *)NIL) {
  switch ((int)(tf->tt)) {
    case nvalpar:  case nvarpar:
   tf = tf->U.V14.tidl;
   break ;
    case nparproc:  case nparfunc:
   tf = tf->U.V15.tparid;
   break ;
    default:
   Caseerror(Line);
  }
 }
 eexpr(tp->U.V30.tcall);
 Putchr('(', output);
 tq = tp->U.V30.taparm;
 while (tq != (struct S64 *)NIL) {
  if (Member((unsigned)(tf->tup->tt), Conset[163])) {
   if (tq->tt == ncall)
    printid(tq->U.V30.tcall->U.V43.tsym->U.V6.lid);
   else
    printid(tq->U.V43.tsym->U.V6.lid);
  } else {
   tx = typeof(tq);
   if (tx == typnods.A[(int)(tboolean)]) {
    tx = tq;
    while (tx->tt == nuplus)
     tx = tx->U.V42.texps;
    if (Member((unsigned)(tx->tt), Conset[164])) {
     Putchr('(', output);
     printid(defnams.A[(int)(dboolean)]->U.V6.lid);
     (void)fprintf(output.fp, ")("), Putl(output, 0);
     eexpr(tq);
     Putchr(')', output);
    } else
     if (tf->tup->tt == nvarpar)
      eaddr(tq);
     else
      eexpr(tq);
   } else
    if (tx == typnods.A[(int)(tset)]) {
     (void)fprintf(output.fp, "*(("), Putl(output, 0);
     etypedef(tf->tup->U.V14.tbind);
     (void)fprintf(output.fp, " *)"), Putl(output, 0);
     (*G224_dropset) = true;
     if (align) {
      usesal = true;
      (void)fprintf(output.fp, "SETALIGN("), Putl(output, 0);
      eexpr(tq);
      Putchr(')', output);
     } else
      eexpr(tq);
     (*G224_dropset) = false;
     Putchr(')', output);
    } else
     if (tx == typnods.A[(int)(tstring)]) {
      (void)fprintf(output.fp, "*(("), Putl(output, 0);
      etypedef(tf->tup->U.V14.tbind);
      (void)fprintf(output.fp, " *)"), Putl(output, 0);
      if (align) {
       usealig = true;
       (void)fprintf(output.fp, "STRALIGN("), Putl(output, 0);
       eexpr(tq);
       Putchr(')', output);
      } else
       eexpr(tq);
      Putchr(')', output);
     } else
      if (tx == typnods.A[(int)(tnil)]) {
       Putchr('(', output);
       etypedef(tf->tup->U.V14.tbind);
       (void)fprintf(output.fp, ")NIL"), Putl(output, 0);
      } else
       if (tf->tup->U.V14.tbind->tt == nconfarr) {
        (void)fprintf(output.fp, "(struct "), Putl(output, 0);
        printid(tf->tup->U.V14.tbind->U.V22.tcuid);
        (void)fprintf(output.fp, " *)&"), Putl(output, 0);
        eexpr(tq);
        if (tq->tnext == (struct S64 *)NIL) {
         (void)fprintf(output.fp, ", ("), Putl(output, 0);
         eexpr(tx->U.V23.taindx->U.V19.thi);
         (void)fprintf(output.fp, " - "), Putl(output, 0);
         eexpr(tx->U.V23.taindx->U.V19.tlo);
         (void)fprintf(output.fp, " + 1)"), Putl(output, 0);
        }
       } else {
        if (tf->tup->tt == nvarpar)
         eaddr(tq);
        else
         eexpr(tq);
       }
  }
  tq = tq->tnext;
  if (tq != (struct S64 *)NIL) {
   (void)fprintf(output.fp, ", "), Putl(output, 0);
   if (tf->tnext == (struct S64 *)NIL) {
    tf = tf->tup->tnext;
    switch ((int)(tf->tt)) {
      case nvalpar:  case nvarpar:
     tf = tf->U.V14.tidl;
     break ;
      case nparproc:  case nparfunc:
     tf = tf->U.V15.tparid;
     break ;
      default:
     Caseerror(Line);
    }
   } else
    tf = tf->tnext;
  }
 }
 Putchr(')', output);
}

void eexpr();

boolean constset();

 boolean
constxps(tp)
 treeptr tp;
{
 register boolean R199;

 switch ((int)(tp->tt)) {
   case nrange:
  if (constxps(tp->U.V41.texpr))
   R199 = constxps(tp->U.V41.texpl);
  else
   R199 = false;
  break ;
   case nempty:  case ninteger:  case nchar:
  R199 = true;
  break ;
   case nid:
  tp = idup(tp);
  R199 = (boolean)((tp->tt == nconst) || (tp->tt == nscalar));
  break ;
   case nin:  case neq:  case nne:  case nlt:
   case nle:  case ngt:  case nge:  case nor:
   case nplus:  case nminus:  case nand:  case nmul:
   case ndiv:  case nmod:  case nquot:  case nnot:
   case numinus:  case nuplus:  case nset:  case nindex:
   case nselect:  case nderef:  case ncall:  case nreal:
   case nstring:  case nnil:
  R199 = false;
  break ;
   default:
  Caseerror(Line);
 }
 return R199;
}

 boolean
constset(tp)
 treeptr tp;
{
 register boolean R198;

 R198 = true;
 while (tp != (struct S64 *)NIL)
  if (constxps(tp))
   tp = tp->tnext;
  else {
   R198 = false;
   tp = (struct S64 *)NIL;
  }
 return R198;
}

 void
eexpr(tp)
 treeptr tp;
{
 treeptr tq;
 boolean flag;

 (*G228_donearr) = false;
 if (Member((unsigned)(tp->tt), Conset[165])) {
  tq = typeof(tp->U.V41.texpl);
  if ((Member((unsigned)(tq->tt), Conset[166])) || (tq == typnods.A[(int)(tset)])) {
   switch ((int)(tp->tt)) {
     case nplus:
    (*G222_setused) = true;
    useunion = true;
    (void)fprintf(output.fp, "Union"), Putl(output, 0);
    break ;
     case nminus:
    (*G222_setused) = true;
    usediff = true;
    (void)fprintf(output.fp, "Diff"), Putl(output, 0);
    break ;
     case nmul:
    (*G222_setused) = true;
    useintr = true;
    (void)fprintf(output.fp, "Inter"), Putl(output, 0);
    break ;
     case neq:
    useseq = true;
    (void)fprintf(output.fp, "Eq"), Putl(output, 0);
    break ;
     case nne:
    usesne = true;
    (void)fprintf(output.fp, "Ne"), Putl(output, 0);
    break ;
     case nge:
    usesge = true;
    (void)fprintf(output.fp, "Ge"), Putl(output, 0);
    break ;
     case nle:
    usesle = true;
    (void)fprintf(output.fp, "Le"), Putl(output, 0);
    break ;
     default:
    Caseerror(Line);
   }
   if (Member((unsigned)(tp->tt), Conset[167]))
    (*G224_dropset) = false;
   Putchr('(', output);
   eexpr(tp->U.V41.texpl);
   if (tq->tt == nsetof)
    (void)fprintf(output.fp, ".S"), Putl(output, 0);
   (void)fprintf(output.fp, ", "), Putl(output, 0);
   eexpr(tp->U.V41.texpr);
   tq = typeof(tp->U.V41.texpr);
   if (tq->tt == nsetof)
    (void)fprintf(output.fp, ".S"), Putl(output, 0);
   Putchr(')', output);
   goto L999;
  }
 }
 if (Member((unsigned)(tp->tt), Conset[168])) {
  tq = typeof(tp->U.V41.texpl);
  if (tq->tt == nconfarr)
   fatal(ecmpconf);
  if ((Member((unsigned)(tq->tt), Conset[169])) || (tq == typnods.A[(int)(tstring)])) {
   (void)fprintf(output.fp, "Cmpstr("), Putl(output, 0);
   eexpr(tp->U.V41.texpl);
   if (tq->tt == narray)
    (void)fprintf(output.fp, ".A"), Putl(output, 0);
   (void)fprintf(output.fp, ", "), Putl(output, 0);
   tq = typeof(tp->U.V41.texpr);
   if (tq->tt == nconfarr)
    fatal(ecmpconf);
   eexpr(tp->U.V41.texpr);
   if (tq->tt == narray)
    (void)fprintf(output.fp, ".A"), Putl(output, 0);
   Putchr(')', output);
   switch ((int)(tp->tt)) {
     case neq:
    (void)fprintf(output.fp, " == "), Putl(output, 0);
    break ;
     case nne:
    (void)fprintf(output.fp, " != "), Putl(output, 0);
    break ;
     case ngt:
    (void)fprintf(output.fp, " > "), Putl(output, 0);
    break ;
     case nlt:
    (void)fprintf(output.fp, " < "), Putl(output, 0);
    break ;
     case nge:
    (void)fprintf(output.fp, " >= "), Putl(output, 0);
    break ;
     case nle:
    (void)fprintf(output.fp, " <= "), Putl(output, 0);
    break ;
     default:
    Caseerror(Line);
   }
   Putchr('0', output);
   goto L999;
  }
 }
 switch ((int)(tp->tt)) {
   case neq:  case nne:  case nlt:  case nle:
   case ngt:  case nge:  case nor:  case nand:
   case nplus:  case nminus:  case nmul:  case ndiv:
   case nmod:  case nquot:
  flag = (boolean)(cprio.A[(int)(tp->tt) - (int)(nassign)] > cprio.A[(int)(tp->U.V41.texpl->tt) - (int)(nassign)]);
  if ((Member((unsigned)(tp->tt), Conset[170])) && !arithexpr(tp->U.V41.texpl)) {
   (void)fprintf(output.fp, "(int)"), Putl(output, 0);
   flag = true;
  }
  if (flag)
   Putchr('(', output);
  eexpr(tp->U.V41.texpl);
  if (flag)
   Putchr(')', output);
  switch ((int)(tp->tt)) {
    case neq:
   (void)fprintf(output.fp, " == "), Putl(output, 0);
   break ;
    case nne:
   (void)fprintf(output.fp, " != "), Putl(output, 0);
   break ;
    case nlt:
   (void)fprintf(output.fp, " < "), Putl(output, 0);
   break ;
    case nle:
   (void)fprintf(output.fp, " <= "), Putl(output, 0);
   break ;
    case ngt:
   (void)fprintf(output.fp, " > "), Putl(output, 0);
   break ;
    case nge:
   (void)fprintf(output.fp, " >= "), Putl(output, 0);
   break ;
    case nor:
   (void)fprintf(output.fp, " || "), Putl(output, 0);
   break ;
    case nand:
   (void)fprintf(output.fp, " && "), Putl(output, 0);
   break ;
    case nplus:
   (void)fprintf(output.fp, " + "), Putl(output, 0);
   break ;
    case nminus:
   (void)fprintf(output.fp, " - "), Putl(output, 0);
   break ;
    case nmul:
   (void)fprintf(output.fp, " * "), Putl(output, 0);
   break ;
    case ndiv:
   (void)fprintf(output.fp, " / "), Putl(output, 0);
   break ;
    case nmod:
   (void)fprintf(output.fp, " %% "), Putl(output, 0);
   break ;
    case nquot:
   (void)fprintf(output.fp, " / (("), Putl(output, 0);
   printid(defnams.A[(int)(dreal)]->U.V6.lid);
   Putchr(')', output);
   break ;
    default:
   Caseerror(Line);
  }
  flag = (boolean)(cprio.A[(int)(tp->tt) - (int)(nassign)] > cprio.A[(int)(tp->U.V41.texpr->tt) - (int)(nassign)]);
  if ((Member((unsigned)(tp->tt), Conset[171])) && !arithexpr(tp->U.V41.texpr)) {
   (void)fprintf(output.fp, "(int)"), Putl(output, 0);
   flag = true;
  }
  if (flag)
   Putchr('(', output);
  eexpr(tp->U.V41.texpr);
  if (flag)
   Putchr(')', output);
  if (tp->tt == nquot)
   Putchr(')', output);
  break ;
   case nuplus:  case numinus:  case nnot:
  switch ((int)(tp->tt)) {
    case numinus:
   Putchr('-', output);
   break ;
    case nnot:
   Putchr('!', output);
   break ;
    case nuplus:
   break ;
    default:
   Caseerror(Line);
  }
  flag = (boolean)(cprio.A[(int)(tp->tt) - (int)(nassign)] >= cprio.A[(int)(tp->U.V42.texps->tt) - (int)(nassign)]);
  if (flag)
   Putchr('(', output);
  eexpr(tp->U.V42.texps);
  if (flag)
   Putchr(')', output);
  break ;
   case nin:
  usememb = true;
  (void)fprintf(output.fp, "Member((unsigned)("), Putl(output, 0);
  eexpr(tp->U.V41.texpl);
  (void)fprintf(output.fp, "), "), Putl(output, 0);
  (*G224_dropset) = true;
  eexpr(tp->U.V41.texpr);
  (*G224_dropset) = false;
  tq = typeof(tp->U.V41.texpr);
  if (tq->tt == nsetof)
   (void)fprintf(output.fp, ".S"), Putl(output, 0);
  Putchr(')', output);
  break ;
   case nassign:
  tq = typeof(tp->U.V27.trhs);
  if (tq == typnods.A[(int)(tstring)]) {
   (void)fprintf(output.fp, "%sstrncpy(", voidcast), Putl(output, 0);
   eexpr(tp->U.V27.tlhs);
   (void)fprintf(output.fp, ".A, "), Putl(output, 0);
   eexpr(tp->U.V27.trhs);
   (void)fprintf(output.fp, ", sizeof("), Putl(output, 0);
   eexpr(tp->U.V27.tlhs);
   (void)fprintf(output.fp, ".A))"), Putl(output, 0);
  } else
   if (tq == typnods.A[(int)(tboolean)]) {
    eexpr(tp->U.V27.tlhs);
    (void)fprintf(output.fp, " = "), Putl(output, 0);
    tq = tp->U.V27.trhs;
    while (tq->tt == nuplus)
     tq = tq->U.V42.texps;
    if (Member((unsigned)(tq->tt), Conset[172])) {
     Putchr('(', output);
     printid(defnams.A[(int)(dboolean)]->U.V6.lid);
     (void)fprintf(output.fp, ")("), Putl(output, 0);
     eexpr(tq);
     Putchr(')', output);
    } else
     eexpr(tq);
   } else
    if (tq == typnods.A[(int)(tnil)]) {
     eexpr(tp->U.V27.tlhs);
     (void)fprintf(output.fp, " = ("), Putl(output, 0);
     etypedef(typeof(tp->U.V27.tlhs));
     (void)fprintf(output.fp, ")NIL"), Putl(output, 0);
    } else {
     tq = typeof(tp->U.V27.tlhs);
     if (tq->tt == nsetof) {
      usescpy = true;
      (void)fprintf(output.fp, "Setncpy("), Putl(output, 0);
      eselect(tp->U.V27.tlhs);
      (void)fprintf(output.fp, "S, "), Putl(output, 0);
      (*G224_dropset) = true;
      tq = typeof(tp->U.V27.trhs);
      if (tq == typnods.A[(int)(tset)])
       eexpr(tp->U.V27.trhs);
      else {
       eselect(tp->U.V27.trhs);
       Putchr('S', output);
      }
      (*G224_dropset) = false;
      (void)fprintf(output.fp, ", sizeof("), Putl(output, 0);
      eselect(tp->U.V27.tlhs);
      (void)fprintf(output.fp, "S))"), Putl(output, 0);
     } else {
      eexpr(tp->U.V27.tlhs);
      (void)fprintf(output.fp, " = "), Putl(output, 0);
      eexpr(tp->U.V27.trhs);
     }
    }
  break ;
   case ncall:
  tq = idup(tp->U.V30.tcall);
  if ((Member((unsigned)(tq->tt), Conset[173])) && (tq->U.V13.tsubstmt != (struct S64 *)NIL))
   if (tq->U.V13.tsubstmt->tt == npredef)
    epredef(tq, tp);
   else
    ecall(tp);
  else
   ecall(tp);
  break ;
   case nselect:
  eselect(tp->U.V40.trecord);
  eexpr(tp->U.V40.tfield);
  break ;
   case nindex:
  eselect(tp->U.V39.tvariable);
  (void)fprintf(output.fp, "A["), Putl(output, 0);
  tq = tp->U.V39.toffset;
  if (arithexpr(tq))
   eexpr(tq);
  else {
   (void)fprintf(output.fp, "(int)("), Putl(output, 0);
   eexpr(tq);
   Putchr(')', output);
  }
  tq = typeof(tp->U.V39.tvariable);
  if (tq->tt == narray)
   if (clower(tq->U.V23.taindx) != 0) {
    (void)fprintf(output.fp, " - "), Putl(output, 0);
    tq = typeof(tq->U.V23.taindx);
    if (tq->tt == nsubrange)
     if (arithexpr(tq->U.V19.tlo))
      eexpr(tq->U.V19.tlo);
     else {
      (void)fprintf(output.fp, "(int)("), Putl(output, 0);
      eexpr(tq->U.V19.tlo);
      Putchr(')', output);
     }
    else
     fatal(etree);
   }
  Putchr(']', output);
  break ;
   case nderef:
  tq = typeof(tp->U.V42.texps);
  if ((tq->tt == nfileof) || ((tq->tt == npredef) && (tq->U.V12.tdef == dtext))) {
   eexpr(tp->U.V42.texps);
   (void)fprintf(output.fp, ".buf"), Putl(output, 0);
  } else
   if ((*G226_doarrow)) {
    (*G226_doarrow) = false;
    eexpr(tp->U.V42.texps);
    (void)fprintf(output.fp, "->"), Putl(output, 0);
    (*G228_donearr) = true;
   } else {
    (void)fprintf(output.fp, "(*"), Putl(output, 0);
    eexpr(tp->U.V42.texps);
    Putchr(')', output);
   }
  break ;
   case nid:
  tq = idup(tp);
  if (tq->tt == nvarpar)
   if ((*G226_doarrow)) {
    (*G226_doarrow) = false;
    printid(tp->U.V43.tsym->U.V6.lid);
    (void)fprintf(output.fp, "->"), Putl(output, 0);
    (*G228_donearr) = true;
   } else {
    (void)fprintf(output.fp, "(*"), Putl(output, 0);
    printid(tp->U.V43.tsym->U.V6.lid);
    Putchr(')', output);
   }
  else
   if ((tq->tt == nconst) && (*G220_conflag))
    (void)fprintf(output.fp, "%1d", cvalof(tp)), Putl(output, 0);
   else
    if (Member((unsigned)(tq->tt), Conset[174])) {
     (void)fprintf(output.fp, "(*"), Putl(output, 0);
     printid(tp->U.V43.tsym->U.V6.lid);
     Putchr(')', output);
    } else
     printid(tp->U.V43.tsym->U.V6.lid);
  break ;
   case nchar:
  printchr(tp->U.V43.tsym->U.V11.lchar);
  break ;
   case ninteger:
  (void)fprintf(output.fp, "%1d", tp->U.V43.tsym->U.V10.linum), Putl(output, 0);
  break ;
   case nreal:
  printtok(tp->U.V43.tsym->U.V8.lfloat);
  break ;
   case nstring:
  printstr(tp->U.V43.tsym->U.V7.lstr);
  break ;
   case nset:
  if (constset(tp->U.V42.texps)) {
   (void)fprintf(output.fp, "Conset[%1d]", setcnt), Putl(output, 0);
   setcnt = setcnt + 1;
   tq = mknode(nset);
   tq->tnext = setlst;
   setlst = tq;
   tq->U.V42.texps = tp->U.V42.texps;
  } else {
   increment();
   flag = (*G224_dropset);
   if ((*G224_dropset))
    (*G224_dropset) = false;
   else
    (void)fprintf(output.fp, "Saveset("), Putl(output, 0);
   (void)fprintf(output.fp, "(Tmpset = Newset(), "), Putl(output, 0);
   tq = tp->U.V42.texps;
   while (tq != (struct S64 *)NIL) {
    switch ((int)(tq->tt)) {
      case nrange:
     usemksub = true;
     (void)fprintf(output.fp, "%sMksubr(", voidcast), Putl(output, 0);
     (void)fprintf(output.fp, "(unsigned)("), Putl(output, 0);
     eexpr(tq->U.V41.texpl);
     (void)fprintf(output.fp, "), "), Putl(output, 0);
     (void)fprintf(output.fp, "(unsigned)("), Putl(output, 0);
     eexpr(tq->U.V41.texpr);
     (void)fprintf(output.fp, "), Tmpset)"), Putl(output, 0);
     break ;
      case nin:  case neq:  case nne:  case nlt:
      case nle:  case ngt:  case nge:  case nor:
      case nand:  case nmul:  case ndiv:  case nmod:
      case nquot:  case nplus:  case nminus:  case nnot:
      case numinus:  case nuplus:  case nindex:  case nselect:
      case nderef:  case ncall:  case ninteger:  case nchar:
      case nid:
     useins = true;
     (void)fprintf(output.fp, "%sInsmem(", voidcast), Putl(output, 0);
     (void)fprintf(output.fp, "(unsigned)("), Putl(output, 0);
     eexpr(tq);
     (void)fprintf(output.fp, "), Tmpset)"), Putl(output, 0);
     break ;
      default:
     Caseerror(Line);
    }
    tq = tq->tnext;
    if (tq != (struct S64 *)NIL) {
     Putchr(',', output),Putchr('\n', output);
     indent();
    }
   }
   (void)fprintf(output.fp, ", Tmpset)"), Putl(output, 0);
   if (!flag) {
    Putchr(')', output);
    (*G222_setused) = true;
   }
   decrement();
  }
  break ;
   case nnil:
  tq = tp;
  do {
   tq = tq->tup;
  } while (!(Member((unsigned)(tq->tt), Conset[175])));
  if (Member((unsigned)(tq->tt), Conset[176])) {
   if (typeof(tq->U.V41.texpl) == typnods.A[(int)(tnil)])
    tq = typeof(tq->U.V41.texpr);
   else
    tq = typeof(tq->U.V41.texpl);
   if (tq->tt == nptr) {
    Putchr('(', output);
    etypedef(tq);
    Putchr(')', output);
   }
  }
  (void)fprintf(output.fp, "NIL"), Putl(output, 0);
  break ;
   default:
  Caseerror(Line);
 }
L999:
 ;
}

 void
econst(tp)
 treeptr tp;
{
 symptr sp;

 while (tp != (struct S64 *)NIL) {
  sp = tp->U.V14.tidl->U.V43.tsym;
  if (sp->U.V6.lid->inref > 1)
   sp->U.V6.lid = mkrename('X', sp->U.V6.lid);
  if (tp->U.V14.tbind->tt == nstring) {
   indent();
   (void)fprintf(output.fp, "%s%s%c", C50_static, chartyp, tab1), Putl(output, 0);
   printid(sp->U.V6.lid);
   (void)fprintf(output.fp, "[]       = "), Putl(output, 0);
   eexpr(tp->U.V14.tbind);
   Putchr(';', output),Putchr('\n', output);
  } else {
   (void)fprintf(output.fp, "%s", C4_define), Putl(output, 0);
   printid(sp->U.V6.lid);
   Putchr(space, output);
   eexpr(tp->U.V14.tbind);
   Putchr('\n', output);
  }
  tp = tp->tnext;
 }
}

 void
edconst(tp)
 treeptr tp;
{
 symptr sp;

 while (tp != (struct S64 *)NIL) {
  sp = tp->U.V14.tidl->U.V43.tsym;
  if (tp->U.V14.tbind->tt != nstring) {
   (void)fprintf(output.fp, "%s", C53_undef), Putl(output, 0);
   printid(sp->U.V6.lid);
   Putchr('\n', output);
  }
  tp = tp->tnext;
 }
}

void etypedef();

void etdef();

 void
etrange(tp)
 treeptr tp;
{
 integer lo, hi;
 register unsigned char i;

 lo = clower(tp);
 hi = cupper(tp);
 {
  unsigned char B51 = 1,
   B52 = nmachdefs;

  if (B51 <= B52)
   for (i = B51; ; i++) {
    {
     register struct S219 *W53 = &machdefs.A[i - 1];

     if ((lo >= W53->lolim) && (hi <= W53->hilim)) {
      printtok(W53->typstr);
      goto L999;
     }
    }
    if (i == B52) break;
   }
 }
 fatal(erange);
L999:
 ;
}

 void
printsuf(ip)
 idptr ip;
{
 toknbuf w;
 toknidx i, j;

 gettokn(ip->istr, &w);
 i = 1;
 j = i;
 while (w.A[i - 1] != null) {
  if (w.A[i - 1] == '.')
   j = i;
  i = i + 1;
 }
 if (w.A[j - 1] == '.')
  j = j + 1;
 while (w.A[j - 1] != null) {
  Putchr(w.A[j - 1], output);
  j = j + 1;
 }
}

 void
etdef(uid, tp)
 idptr uid;
 treeptr tp;
{
 integer i;
 treeptr tq;

 switch ((int)(tp->tt)) {
   case nid:
  printid(tp->U.V43.tsym->U.V6.lid);
  break ;
   case nptr:
  tq = typeof(tp->U.V16.tptrid);
  if (tq->tt == nrecord) {
   (void)fprintf(output.fp, "struct "), Putl(output, 0);
   printid(tq->U.V21.tuid);
  } else
   printid(tp->U.V16.tptrid->U.V43.tsym->U.V6.lid);
  (void)fprintf(output.fp, " *"), Putl(output, 0);
  break ;
   case nscalar:
  (void)fprintf(output.fp, "enum { "), Putl(output, 0);
  increment();
  tp = tp->U.V17.tscalid;
  if (tp->U.V43.tsym->U.V6.lid->inref > 1)
   tp->U.V43.tsym->U.V6.lid = mkrename('E', tp->U.V43.tsym->U.V6.lid);
  printid(tp->U.V43.tsym->U.V6.lid);
  i = 1;
  while (tp->tnext != (struct S64 *)NIL) {
   if (i >= 4) {
    Putchr(',', output),Putchr('\n', output);
    indent();
    i = 1;
   } else {
    (void)fprintf(output.fp, ", "), Putl(output, 0);
    i = i + 1;
   }
   tp = tp->tnext;
   if (tp->U.V43.tsym->U.V6.lid->inref > 1)
    tp->U.V43.tsym->U.V6.lid = mkrename('E', tp->U.V43.tsym->U.V6.lid);
   printid(tp->U.V43.tsym->U.V6.lid);
  }
  decrement();
  (void)fprintf(output.fp, " } "), Putl(output, 0);
  break ;
   case nsubrange:
  tq = typeof(tp->U.V19.tlo);
  if (tq == typnods.A[(int)(tinteger)])
   etrange(tp);
  else {
   if (tq->tup->tt == ntype)
    tq = tq->tup->U.V14.tidl;
   etdef((idptr)NIL, tq);
  }
  break ;
   case nfield:
  etdef((idptr)NIL, tp->U.V14.tbind);
  Putchr(tab1, output);
  tp = tp->U.V14.tidl;
  if (uid != (struct S62 *)NIL)
   tp->U.V43.tsym->U.V6.lid = mkconc('.', uid, tp->U.V43.tsym->U.V6.lid);
  printsuf(tp->U.V43.tsym->U.V6.lid);
  i = 1;
  while (tp->tnext != (struct S64 *)NIL) {
   if (i >= 4) {
    Putchr(',', output),Putchr('\n', output);
    indent();
    Putchr(tab1, output);
    i = 1;
   } else {
    (void)fprintf(output.fp, ", "), Putl(output, 0);
    i = i + 1;
   }
   tp = tp->tnext;
   if (uid != (struct S62 *)NIL)
    tp->U.V43.tsym->U.V6.lid = mkconc('.', uid, tp->U.V43.tsym->U.V6.lid);
   printsuf(tp->U.V43.tsym->U.V6.lid);
  }
  Putchr(';', output),Putchr('\n', output);
  break ;
   case nrecord:
  (void)fprintf(output.fp, "struct "), Putl(output, 0);
  if (tp->U.V21.tuid == (struct S62 *)NIL)
   tp->U.V21.tuid = uid;
  else
   if (uid == (struct S62 *)NIL)
    printid(tp->U.V21.tuid);
  (void)fprintf(output.fp, " {\n"), Putl(output, 1);
  increment();
  if ((tp->U.V21.tflist == (struct S64 *)NIL) && (tp->U.V21.tvlist == (struct S64 *)NIL)) {
   indent();
   (void)fprintf(output.fp, "%s%cdummy;\n", inttyp, tab1), Putl(output, 1);
  }
  tq = tp->U.V21.tflist;
  while (tq != (struct S64 *)NIL) {
   indent();
   etdef(uid, tq);
   tq = tq->tnext;
  }
  if (tp->U.V21.tvlist != (struct S64 *)NIL) {
   indent();
   (void)fprintf(output.fp, "union {\n"), Putl(output, 1);
   increment();
   tq = tp->U.V21.tvlist;
   while (tq != (struct S64 *)NIL) {
    if ((tq->U.V20.tvrnt->U.V21.tflist != (struct S64 *)NIL) || (tq->U.V20.tvrnt->U.V21.tvlist != (struct S64 *)NIL)) {
     indent();
     if (uid == (struct S62 *)NIL)
      etdef(mkvrnt(), tq->U.V20.tvrnt);
     else
      etdef(mkconc('.', uid, mkvrnt()), tq->U.V20.tvrnt);
     Putchr(';', output),Putchr('\n', output);
    }
    tq = tq->tnext;
   }
   decrement();
   indent();
   (void)fprintf(output.fp, "} U;\n"), Putl(output, 1);
  }
  decrement();
  indent();
  if (tp->tup->tt == nvariant) {
   (void)fprintf(output.fp, "} "), Putl(output, 0);
   printsuf(tp->U.V21.tuid);
  } else
   Putchr('}', output);
  break ;
   case nconfarr:
  (void)fprintf(output.fp, "struct "), Putl(output, 0);
  printid(tp->U.V22.tcuid);
  (void)fprintf(output.fp, " { "), Putl(output, 0);
  etdef((idptr)NIL, tp->U.V22.tcelem);
  (void)fprintf(output.fp, "%cA[]; }", tab1), Putl(output, 0);
  break ;
   case narray:
  (void)fprintf(output.fp, "struct { "), Putl(output, 0);
  etdef((idptr)NIL, tp->U.V23.taelem);
  (void)fprintf(output.fp, "%cA[", tab1), Putl(output, 0);
  tq = typeof(tp->U.V23.taindx);
  if (tq->tt == nsubrange) {
   if (arithexpr(tq->U.V19.thi)) {
    eexpr(tq->U.V19.thi);
    if (cvalof(tq->U.V19.tlo) != 0) {
     (void)fprintf(output.fp, " - "), Putl(output, 0);
     eexpr(tq->U.V19.tlo);
    }
   } else {
    (void)fprintf(output.fp, "(int)("), Putl(output, 0);
    eexpr(tq->U.V19.thi);
    if (cvalof(tq->U.V19.tlo) != 0) {
     (void)fprintf(output.fp, ") - (int)("), Putl(output, 0);
     eexpr(tq->U.V19.tlo);
    }
    Putchr(')', output);
   }
   (void)fprintf(output.fp, " + 1"), Putl(output, 0);
  } else
   (void)fprintf(output.fp, "%1d", crange(tp->U.V23.taindx)), Putl(output, 0);
  (void)fprintf(output.fp, "]; }"), Putl(output, 0);
  break ;
   case nfileof:
  (void)fprintf(output.fp, "struct {\n"), Putl(output, 1);
  indent();
  (void)fprintf(output.fp, "%cFILE%c*fp;\n", tab1, tab1), Putl(output, 1);
  indent();
  (void)fprintf(output.fp, "%c%s%ceoln:1,\n", tab1, filebits, tab1), Putl(output, 1);
  indent();
  (void)fprintf(output.fp, "%seof:1,\n", tab3), Putl(output, 1);
  indent();
  (void)fprintf(output.fp, "%sout:1,\n", tab3), Putl(output, 1);
  indent();
  (void)fprintf(output.fp, "%sinit:1,\n", tab3), Putl(output, 1);
  indent();
  (void)fprintf(output.fp, "%s:%1d;\n", tab3, filefill), Putl(output, 1);
  indent();
  Putchr(tab1, output);
  etdef((idptr)NIL, tp->U.V18.tof);
  (void)fprintf(output.fp, "%cbuf;\n", tab1), Putl(output, 1);
  indent();
  (void)fprintf(output.fp, "} "), Putl(output, 0);
  break ;
   case nsetof:
  (void)fprintf(output.fp, "struct { %s%cS[%1d]; }", setwtyp, tab1, csetsize(tp)), Putl(output, 0);
  break ;
   case npredef:
  switch ((int)(tp->U.V12.tobtyp)) {
    case tboolean:
   printid(defnams.A[(int)(dboolean)]->U.V6.lid);
   break ;
    case tchar:
   (void)fprintf(output.fp, "%s", chartyp), Putl(output, 0);
   break ;
    case tinteger:
   printid(defnams.A[(int)(dinteger)]->U.V6.lid);
   break ;
    case treal:
   printid(defnams.A[(int)(dreal)]->U.V6.lid);
   break ;
    case tstring:
   (void)fprintf(output.fp, "%s *", chartyp), Putl(output, 0);
   break ;
    case ttext:
   (void)fprintf(output.fp, "text"), Putl(output, 0);
   break ;
    case tnil:  case tset:  case terror:
   fatal(etree);
   break ;
    case tnone:
   (void)fprintf(output.fp, "%s", voidtyp), Putl(output, 0);
   break ;
    default:
   Caseerror(Line);
  }
  break ;
   case nempty:
  (void)fprintf(output.fp, "%s", voidtyp), Putl(output, 0);
  break ;
   default:
  Caseerror(Line);
 }
}

 void
etypedef(tp)
 treeptr tp;
{
 etdef((idptr)NIL, tp);
}

 void
etype(tp)
 treeptr tp;
{
 symptr sp;

 while (tp != (struct S64 *)NIL) {
  sp = tp->U.V14.tidl->U.V43.tsym;
  if (sp->U.V6.lid->inref > 1)
   sp->U.V6.lid = mkrename('Y', sp->U.V6.lid);
  indent();
  (void)fprintf(output.fp, "%s", typdef), Putl(output, 0);
  etypedef(tp->U.V14.tbind);
  Putchr(tab1, output);
  printid(sp->U.V6.lid);
  Putchr(';', output),Putchr('\n', output);
  tp = tp->tnext;
 }
}

 void
evar(tp)
 treeptr tp;
{
 treeptr tq;
 integer i;

 while (tp != (struct S64 *)NIL) {
  indent();
  switch ((int)(tp->tt)) {
    case nvar:  case nvalpar:  case nvarpar:
   if (tp->U.V14.tattr == aregister)
    (void)fprintf(output.fp, "%s", registr), Putl(output, 0);
   etypedef(tp->U.V14.tbind);
   break ;
    case nparproc:  case nparfunc:
   if (tp->tt == nparproc)
    (void)fprintf(output.fp, "%s", voidtyp), Putl(output, 0);
   else
    etypedef(tp->U.V15.tpartyp);
   tq = tp->U.V15.tparid;
   (void)fprintf(output.fp, "%c(*", tab1), Putl(output, 0);
   printid(tq->U.V43.tsym->U.V6.lid);
   (void)fprintf(output.fp, ")()"), Putl(output, 0);
   goto L555;
   break ;
    default:
   Caseerror(Line);
  }
  Putchr(tab1, output);
  tq = tp->U.V14.tidl;
  i = 1;
  do {
   if (tp->tt == nvarpar)
    Putchr('*', output);
   printid(tq->U.V43.tsym->U.V6.lid);
   tq = tq->tnext;
   if (tq != (struct S64 *)NIL) {
    if (i >= 6) {
     i = 1;
     Putchr(',', output),Putchr('\n', output);
     indent();
     Putchr(tab1, output);
    } else {
     i = i + 1;
     (void)fprintf(output.fp, ", "), Putl(output, 0);
    }
   }
  } while (!(tq == (struct S64 *)NIL));
 L555:
  Putchr(';', output),Putchr('\n', output);
  if (tp->tt == nvarpar)
   if (tp->U.V14.tbind->tt == nconfarr) {
    indent();
    etypedef(tp->U.V14.tbind->U.V22.tindtyp);
    Putchr(tab1, output);
    tq = tp->U.V14.tbind->U.V22.tcindx->U.V19.thi;
    printid(tq->U.V43.tsym->U.V6.lid);
    Putchr(';', output),Putchr('\n', output);
   }
  tp = tp->tnext;
 }
}

void estmt();

 void
ewithtype(tp)
 treeptr tp;
{
 treeptr tq;

 tq = typeof(tp);
 (void)fprintf(output.fp, "struct "), Putl(output, 0);
 printid(tq->U.V21.tuid);
}

 void
echoise(tp)
 treeptr tp;
{
 treeptr tq;
 integer i;

 while (tp != (struct S64 *)NIL) {
  tq = tp->U.V36.tchocon;
  i = 0;
  indent();
  while (tq != (struct S64 *)NIL) {
   (void)fprintf(output.fp, "  case "), Putl(output, 0);
   (*G220_conflag) = true;
   eexpr(tq);
   (*G220_conflag) = false;
   Putchr(':', output);
   i = i + 1;
   tq = tq->tnext;
   if ((tq == (struct S64 *)NIL) || (i % 4 == 0)) {
    Putchr('\n', output);
    if (tq != (struct S64 *)NIL)
     indent();
    i = 0;
   }
  }
  increment();
  if (tp->U.V36.tchostmt->tt == nbegin)
   estmt(tp->U.V36.tchostmt->U.V24.tbegin);
  else
   estmt(tp->U.V36.tchostmt);
  indent();
  (void)fprintf(output.fp, "break ;\n"), Putl(output, 1);
  decrement();
  tp = tp->tnext;
  if (tp != (struct S64 *)NIL)
   if (tp->U.V36.tchocon == (struct S64 *)NIL)
    tp = (struct S64 *)NIL;
 }
}

 void
cenv(ip, dp)
 idptr ip;
 declptr dp;
{
 treeptr tp;
 symptr sp;
 idptr np;
 register hashtyp h;

 {
  register struct S63 *W54 = &(*dp);

  {
   hashtyp B55 = 0,
    B56 = hashmax - 1;

   if (B55 <= B56)
    for (h = B55; ; h++) {
     sp = W54->ddecl.A[h];
     while (sp != (struct S65 *)NIL) {
      if (sp->lt == lfield) {
       np = sp->U.V6.lid;
       tp = sp->lsymdecl->tup->tup;
       if ((tp->tup->tt == nvariant) && (tp->U.V21.tuid != (struct S62 *)NIL))
        np = mkconc('.', tp->U.V21.tuid, np);
       np = mkconc('>', ip, np);
       sp->U.V6.lid = np;
      }
      sp = sp->lnext;
     }
     if (h == B56) break;
    }
  }
 }
}

 void
eglobid(tp)
 treeptr tp;
{
 toknidx j;
 toknbuf w;

 gettokn(tp->U.V43.tsym->U.V6.lid->istr, &w);
 j = 1;
 if (w.A[1 - 1] == '*')
  j = 2;
 while (w.A[j - 1] != null) {
  Putchr(w.A[j - 1], output);
  j = j + 1;
 }
}

 void
estmt(tp)
 treeptr tp;
{
 treeptr tq;
 idptr locid1, locid2;
 boolean stusd;
 char opc1, opc2;

 while (tp != (struct S64 *)NIL) {
  switch ((int)(tp->tt)) {
    case nbegin:
   if (Member((unsigned)(tp->tup->tt), Conset[177]))
    indent();
   Putchr('{', output),Putchr('\n', output);
   increment();
   estmt(tp->U.V24.tbegin);
   decrement();
   indent();
   Putchr('}', output);
   if (tp->tup->tt != nif)
    Putchr('\n', output);
   break ;
    case nrepeat:
   indent();
   (void)fprintf(output.fp, "do {\n"), Putl(output, 1);
   increment();
   estmt(tp->U.V33.treptstmt);
   decrement();
   indent();
   (void)fprintf(output.fp, "} while (!("), Putl(output, 0);
   eexpr(tp->U.V33.treptxp);
   (void)fprintf(output.fp, "));\n"), Putl(output, 1);
   break ;
    case nwhile:
   indent();
   (void)fprintf(output.fp, "while ("), Putl(output, 0);
   increment();
   eexpr(tp->U.V32.twhixp);
   stusd = (*G222_setused);
   if (tp->U.V32.twhistmt->tt == nbegin) {
    decrement();
    (void)fprintf(output.fp, ") "), Putl(output, 0);
    estmt(tp->U.V32.twhistmt);
   } else {
    Putchr(')', output),Putchr('\n', output);
    estmt(tp->U.V32.twhistmt);
    decrement();
   }
   (*G222_setused) = (boolean)(stusd || (*G222_setused));
   break ;
    case nfor:
   indent();
   if (tp->U.V34.tincr) {
    opc1 = '+';
    opc2 = '<';
   } else {
    opc1 = '-';
    opc2 = '>';
   }
   if (!lazyfor) {
    locid1 = mkvariable('B');
    locid2 = mkvariable('B');
    Putchr('{', output),Putchr('\n', output);
    increment();
    indent();
    tq = idup(tp->U.V34.tforid);
    etypedef(tq->U.V14.tbind);
    tq = typeof(tq->U.V14.tbind);
    Putchr(tab1, output);
    printid(locid1);
    (void)fprintf(output.fp, " = "), Putl(output, 0);
    eexpr(tp->U.V34.tfrom);
    Putchr(',', output),Putchr('\n', output);
    indent();
    Putchr(tab1, output);
    printid(locid2);
    (void)fprintf(output.fp, " = "), Putl(output, 0);
    eexpr(tp->U.V34.tto);
    Putchr(';', output),Putchr('\n', output);
    Putchr('\n', output);
    indent();
    (void)fprintf(output.fp, "if ("), Putl(output, 0);
    if (tq->tt == nscalar) {
     (void)fprintf(output.fp, "(int)("), Putl(output, 0);
     printid(locid1);
     Putchr(')', output);
    } else
     printid(locid1);
    (void)fprintf(output.fp, " %c= ", opc2), Putl(output, 0);
    if (tq->tt == nscalar) {
     (void)fprintf(output.fp, "(int)("), Putl(output, 0);
     printid(locid2);
     Putchr(')', output);
    } else
     printid(locid2);
    Putchr(')', output),Putchr('\n', output);
    increment();
    indent();
    tp->U.V34.tfrom = newid(locid1);
    tp->U.V34.tfrom->tup = tp;
   }
   (void)fprintf(output.fp, "for ("), Putl(output, 0);
   increment();
   eexpr(tp->U.V34.tforid);
   tq = typeof(tp->U.V34.tforid);
   (void)fprintf(output.fp, " = "), Putl(output, 0);
   eexpr(tp->U.V34.tfrom);
   (void)fprintf(output.fp, "; "), Putl(output, 0);
   if (lazyfor) {
    if (tq->tt == nscalar) {
     (void)fprintf(output.fp, "(int)("), Putl(output, 0);
     eexpr(tp->U.V34.tforid);
     Putchr(')', output);
    } else
     eexpr(tp->U.V34.tforid);
    (void)fprintf(output.fp, " %c= ", opc2), Putl(output, 0);
    if (tq->tt == nscalar) {
     (void)fprintf(output.fp, "(int)("), Putl(output, 0);
     eexpr(tp->U.V34.tto);
     Putchr(')', output);
    } else
     eexpr(tp->U.V34.tto);
   }
   (void)fprintf(output.fp, "; "), Putl(output, 0);
   eexpr(tp->U.V34.tforid);
   if (tq->tt == nscalar) {
    (void)fprintf(output.fp, " = ("), Putl(output, 0);
    eexpr(tq->tup->U.V14.tidl);
    (void)fprintf(output.fp, ")((int)("), Putl(output, 0);
    eexpr(tp->U.V34.tforid);
    (void)fprintf(output.fp, ")%c1)", opc1), Putl(output, 0);
   } else
    (void)fprintf(output.fp, "%c%c", opc1, opc1), Putl(output, 0);
   if (!lazyfor) {
    if (tp->U.V34.tforstmt->tt != nbegin) {
     tq = mknode(nbegin);
     tq->U.V24.tbegin = tp->U.V34.tforstmt;
     tq->U.V24.tbegin->tup = tq;
     tp->U.V34.tforstmt = tq;
     tq->tup = tp;
    }
    tq = tp->U.V34.tforstmt->U.V24.tbegin;
    while (tq->tnext != (struct S64 *)NIL)
     tq = tq->tnext;
    tq->tnext = mknode(nbreak);
    tq = tq->tnext;
    tq->tup = tp->U.V34.tforstmt;
    tq->U.V29.tbrkid = tp->U.V34.tforid;
    tq->U.V29.tbrkxp = newid(locid2);
    tq->U.V29.tbrkxp->tup = tq;
   }
   if (tp->U.V34.tforstmt->tt == nbegin) {
    decrement();
    (void)fprintf(output.fp, ") "), Putl(output, 0);
    estmt(tp->U.V34.tforstmt);
   } else {
    Putchr(')', output),Putchr('\n', output);
    estmt(tp->U.V34.tforstmt);
    decrement();
   }
   if (!lazyfor) {
    decrement();
    decrement();
    indent();
    Putchr('}', output),Putchr('\n', output);
   }
   break ;
    case nif:
   indent();
   (void)fprintf(output.fp, "if ("), Putl(output, 0);
   increment();
   eexpr(tp->U.V31.tifxp);
   stusd = (*G222_setused);
   (*G222_setused) = false;
   if (tp->U.V31.tthen->tt == nbegin) {
    decrement();
    (void)fprintf(output.fp, ") "), Putl(output, 0);
    estmt(tp->U.V31.tthen);
    if (tp->U.V31.telse != (struct S64 *)NIL)
     Putchr(space, output);
    else
     Putchr('\n', output);
   } else {
    Putchr(')', output),Putchr('\n', output);
    estmt(tp->U.V31.tthen);
    decrement();
    if (tp->U.V31.telse != (struct S64 *)NIL)
     indent();
   }
   if (tp->U.V31.telse != (struct S64 *)NIL) {
    (void)fprintf(output.fp, "else"), Putl(output, 0);
    if (tp->U.V31.telse->tt == nbegin) {
     Putchr(space, output);
     estmt(tp->U.V31.telse);
     Putchr('\n', output);
    } else {
     increment();
     Putchr('\n', output);
     estmt(tp->U.V31.telse);
     decrement();
    }
   }
   (*G222_setused) = (boolean)(stusd || (*G222_setused));
   break ;
    case ncase:
   indent();
   (void)fprintf(output.fp, "switch ((int)("), Putl(output, 0);
   increment();
   eexpr(tp->U.V35.tcasxp);
   (void)fprintf(output.fp, ")) {\n"), Putl(output, 1);
   decrement();
   echoise(tp->U.V35.tcaslst);
   indent();
   (void)fprintf(output.fp, "  default:\n"), Putl(output, 1);
   increment();
   if (tp->U.V35.tcasother == (struct S64 *)NIL) {
    indent();
    (void)fprintf(output.fp, "Caseerror(Line);\n"), Putl(output, 1);
   } else
    estmt(tp->U.V35.tcasother);
   decrement();
   indent();
   Putchr('}', output),Putchr('\n', output);
   break ;
    case nwith:
   indent();
   Putchr('{', output),Putchr('\n', output);
   increment();
   tq = tp->U.V37.twithvar;
   while (tq != (struct S64 *)NIL) {
    indent();
    (void)fprintf(output.fp, "%s", registr), Putl(output, 0);
    ewithtype(tq->U.V38.texpw);
    (void)fprintf(output.fp, " *"), Putl(output, 0);
    locid1 = mkvariable('W');
    printid(locid1);
    (void)fprintf(output.fp, " = "), Putl(output, 0);
    eaddr(tq->U.V38.texpw);
    Putchr(';', output),Putchr('\n', output);
    cenv(locid1, tq->U.V38.tenv);
    tq = tq->tnext;
   }
   Putchr('\n', output);
   if (tp->U.V37.twithstmt->tt == nbegin)
    estmt(tp->U.V37.twithstmt->U.V24.tbegin);
   else
    estmt(tp->U.V37.twithstmt);
   decrement();
   indent();
   Putchr('}', output),Putchr('\n', output);
   break ;
    case ngoto:
   indent();
   if (islocal(tp->U.V26.tlabel))
    (void)fprintf(output.fp, "goto L%1d;\n", tp->U.V26.tlabel->U.V43.tsym->U.V9.lno), Putl(output, 1);
   else {
    tq = idup(tp->U.V26.tlabel);
    (void)fprintf(output.fp, "longjmp(J[%1d].jb, %1d);\n", tq->U.V13.tstat, tp->U.V26.tlabel->U.V43.tsym->U.V9.lno), Putl(output, 1);
   }
   break ;
    case nlabstmt:
   decrement();
   indent();
   (void)fprintf(output.fp, "L%1d:\n", tp->U.V25.tlabno->U.V43.tsym->U.V9.lno), Putl(output, 1);
   increment();
   estmt(tp->U.V25.tstmt);
   break ;
    case nassign:
   indent();
   eexpr(tp);
   Putchr(';', output),Putchr('\n', output);
   break ;
    case ncall:
   indent();
   tq = idup(tp->U.V30.tcall);
   if ((Member((unsigned)(tq->tt), Conset[178])) && (tq->U.V13.tsubstmt != (struct S64 *)NIL))
    if (tq->U.V13.tsubstmt->tt == npredef)
     epredef(tq, tp);
    else {
     ecall(tp);
     Putchr(';', output),Putchr('\n', output);
    }
   else {
    ecall(tp);
    Putchr(';', output),Putchr('\n', output);
   }
   break ;
    case npush:
   indent();
   eglobid(tp->U.V28.ttmp);
   (void)fprintf(output.fp, " = "), Putl(output, 0);
   eglobid(tp->U.V28.tglob);
   Putchr(';', output),Putchr('\n', output);
   indent();
   eglobid(tp->U.V28.tglob);
   (void)fprintf(output.fp, " = "), Putl(output, 0);
   if (tp->U.V28.tloc->tt == nid) {
    tq = idup(tp->U.V28.tloc);
    if (Member((unsigned)(tq->tt), Conset[179]))
     printid(tp->U.V28.tloc->U.V43.tsym->U.V6.lid);
    else
     eaddr(tp->U.V28.tloc);
   } else
    eaddr(tp->U.V28.tloc);
   Putchr(';', output),Putchr('\n', output);
   break ;
    case npop:
   indent();
   eglobid(tp->U.V28.tglob);
   (void)fprintf(output.fp, " = "), Putl(output, 0);
   eglobid(tp->U.V28.ttmp);
   Putchr(';', output),Putchr('\n', output);
   break ;
    case nbreak:
   indent();
   (void)fprintf(output.fp, "if ("), Putl(output, 0);
   eexpr(tp->U.V29.tbrkid);
   (void)fprintf(output.fp, " == "), Putl(output, 0);
   eexpr(tp->U.V29.tbrkxp);
   (void)fprintf(output.fp, ") break;\n"), Putl(output, 1);
   break ;
    case nempty:
   if (!(Member((unsigned)(tp->tup->tt), Conset[180]))) {
    indent();
    Putchr(';', output),Putchr('\n', output);
   }
   break ;
    default:
   Caseerror(Line);
  }
  if ((*G222_setused) && (Member((unsigned)(tp->tup->tt), Conset[181]))) {
   indent();
   (void)fprintf(output.fp, "Claimset();\n"), Putl(output, 1);
   (*G222_setused) = false;
  }
  tp = tp->tnext;
 }
}

 void
elabel(tp)
 treeptr tp;
{
 treeptr tq;
 integer i;

 i = 0;
 tq = tp->U.V13.tsublab;
 while (tq != (struct S64 *)NIL) {
  if (tq->U.V43.tsym->U.V9.lgo)
   i = i + 1;
  tq = tq->tnext;
 }
 if (i == 1) {
  tq = tp->U.V13.tsublab;
  while (!tq->U.V43.tsym->U.V9.lgo)
   tq = tq->tnext;
  indent();
  (void)fprintf(output.fp, "if (setjmp(J[%1d].jb))\n", tp->U.V13.tstat), Putl(output, 1);
  (void)fprintf(output.fp, "%cgoto L%1d;\n", tab1, tq->U.V43.tsym->U.V9.lno), Putl(output, 1);
 } else
  if (i > 1) {
   indent();
   (void)fprintf(output.fp, "switch (setjmp(J[%1d].jb)) {\n", tp->U.V13.tstat), Putl(output, 1);
   indent();
   (void)fprintf(output.fp, "  case 0:\n"), Putl(output, 1);
   indent();
   (void)fprintf(output.fp, "%cbreak;\n", tab1), Putl(output, 1);
   tq = tp->U.V13.tsublab;
   while (tq != (struct S64 *)NIL) {
    if (tq->U.V43.tsym->U.V9.lgo) {
     indent();
     (void)fprintf(output.fp, "  case %1d:\n", tq->U.V43.tsym->U.V9.lno), Putl(output, 1);
     indent();
     (void)fprintf(output.fp, "%cgoto L%1d;\n", tab1, tq->U.V43.tsym->U.V9.lno), Putl(output, 1);
    }
    tq = tq->tnext;
   }
   indent();
   (void)fprintf(output.fp, "  default:\n"), Putl(output, 1);
   indent();
   (void)fprintf(output.fp, "%cCaseerror(Line);\n", tab1), Putl(output, 1);
   indent();
   Putchr('}', output),Putchr('\n', output);
  }
}

 void
econf(tp)
 treeptr tp;
{
 treeptr tq;

 while (tp != (struct S64 *)NIL) {
  if (tp->tt == nvarpar)
   if (tp->U.V14.tbind->tt == nconfarr) {
    indent();
    etypedef(tp->U.V14.tbind->U.V22.tindtyp);
    Putchr(tab1, output);
    tq = tp->U.V14.tbind->U.V22.tcindx->U.V19.tlo;
    printid(tq->U.V43.tsym->U.V6.lid);
    (void)fprintf(output.fp, " = ("), Putl(output, 0);
    etypedef(tp->U.V14.tbind->U.V22.tindtyp);
    (void)fprintf(output.fp, ")0;\n"), Putl(output, 1);
   }
  tp = tp->tnext;
 }
}

 void
esubr(tp)
 treeptr tp;
{
 treeptr tq, ti;

 while (tp != (struct S64 *)NIL) {
  if (tp->U.V13.tsubsub != (struct S64 *)NIL) {
   etypedef(tp->U.V13.tfuntyp);
   Putchr(space, output);
   printid(tp->U.V13.tsubid->U.V43.tsym->U.V6.lid);
   (void)fprintf(output.fp, "();\n"), Putl(output, 1);
   Putchr('\n', output);
   esubr(tp->U.V13.tsubsub);
  }
  if (tp->U.V13.tsubstmt == (struct S64 *)NIL) {
   if (tp->U.V13.tsubid->U.V43.tsym->lsymdecl->tup == tp)
    (void)fprintf(output.fp, "%s", xtern), Putl(output, 0);
   etypedef(tp->U.V13.tfuntyp);
   Putchr(space, output);
   printid(tp->U.V13.tsubid->U.V43.tsym->U.V6.lid);
   (void)fprintf(output.fp, "();\n"), Putl(output, 1);
   goto L999;
  }
  Putchr(space, output);
  etypedef(tp->U.V13.tfuntyp);
  Putchr('\n', output);
  printid(tp->U.V13.tsubid->U.V43.tsym->U.V6.lid);
  Putchr('(', output);
  tq = tp->U.V13.tsubpar;
  while (tq != (struct S64 *)NIL) {
   switch ((int)(tq->tt)) {
     case nvarpar:  case nvalpar:
    ti = tq->U.V14.tidl;
    while (ti != (struct S64 *)NIL) {
     printid(ti->U.V43.tsym->U.V6.lid);
     ti = ti->tnext;
     if (ti != (struct S64 *)NIL)
      (void)fprintf(output.fp, ", "), Putl(output, 0);
    }
    if (tq->U.V14.tbind->tt == nconfarr) {
     ti = tq->U.V14.tbind->U.V22.tcindx->U.V19.thi;
     (void)fprintf(output.fp, ", "), Putl(output, 0);
     printid(ti->U.V43.tsym->U.V6.lid);
    }
    break ;
     case nparproc:  case nparfunc:
    ti = tq->U.V15.tparid;
    printid(ti->U.V43.tsym->U.V6.lid);
    break ;
     default:
    Caseerror(Line);
   }
   tq = tq->tnext;
   if (tq != (struct S64 *)NIL)
    (void)fprintf(output.fp, ", "), Putl(output, 0);
  }
  Putchr(')', output),Putchr('\n', output);
  increment();
  evar(tp->U.V13.tsubpar);
  Putchr('{', output),Putchr('\n', output);
  econf(tp->U.V13.tsubpar);
  econst(tp->U.V13.tsubconst);
  etype(tp->U.V13.tsubtype);
  evar(tp->U.V13.tsubvar);
  if ((tp->U.V13.tsubconst != (struct S64 *)NIL) || (tp->U.V13.tsubtype != (struct S64 *)NIL) || (tp->U.V13.tsubvar != (struct S64 *)NIL))
   Putchr('\n', output);
  elabel(tp);
  estmt(tp->U.V13.tsubstmt);
  if (tp->tt == nfunc) {
   indent();
   (void)fprintf(output.fp, "return "), Putl(output, 0);
   printid(tp->U.V13.tsubvar->U.V14.tidl->U.V43.tsym->U.V6.lid);
   Putchr(';', output),Putchr('\n', output);
  }
  decrement();
  edconst(tp->U.V13.tsubconst);
  Putchr('}', output),Putchr('\n', output);
 L999:
  Putchr('\n', output);
  tp = tp->tnext;
 }
}

 boolean
use(d)
 predefs d;
{
 register boolean R200;

 R200 = defnams.A[(int)(d)]->U.V6.lused;
 return R200;
}

void eprogram();

 void
capital(sp)
 symptr sp;
{
 toknbuf tb;

 if (sp->U.V6.lid->inref > 1) {
  gettokn(sp->U.V6.lid->istr, &tb);
  tb.A[1 - 1] = uppercase(tb.A[1 - 1]);
  sp->U.V6.lid = saveid(&tb);
 }
}

 void
etextdef()
{
 treeptr tq;

 (void)fprintf(output.fp, "typedef "), Putl(output, 0);
 tq = mknode(nfileof);
 tq->U.V18.tof = typnods.A[(int)(tchar)];
 etypedef(tq);
 (void)fprintf(output.fp, "%ctext;\n", tab1), Putl(output, 1);
}

 void
eprogram(tp)
 treeptr tp;
{
 if (tp->U.V13.tsubid != (struct S64 *)NIL) {
  (void)fprintf(output.fp, "/*\n"), Putl(output, 1);
  (void)fprintf(output.fp, "**       Code derived from program "), Putl(output, 0);
  printid(tp->U.V13.tsubid->U.V43.tsym->U.V6.lid);
  Putchr('\n', output);
  (void)fprintf(output.fp, "*/\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%s<stdlib.h>\n", C24_include), Putl(output, 1);
 }
 if (usecase || usesets || use(dinput) || use(doutput) || use(dwrite) || use(dwriteln) || use(dmessage) || use(deof) || use(deoln) || use(dflush) || use(dpage) || use(dread) || use(dreadln) || use(dclose) || use(dreset) || use(drewrite) || use(dget) || use(dput)) {
  (void)fprintf(output.fp, "/*\n"), Putl(output, 1);
  (void)fprintf(output.fp, "**     Definitions for i/o\n"), Putl(output, 1);
  (void)fprintf(output.fp, "*/\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%s<stdio.h>\n", C24_include), Putl(output, 1);
 }
 if (use(dinput) || use(doutput) || use(dtext)) {
  etextdef();
  if (use(dinput)) {
   if (tp->U.V13.tsubid == (struct S64 *)NIL)
    (void)fprintf(output.fp, "%s", xtern), Putl(output, 0);
   (void)fprintf(output.fp, "text%c", tab1), Putl(output, 0);
   printid(defnams.A[(int)(dinput)]->U.V6.lid);
   if (tp->U.V13.tsubid != (struct S64 *)NIL)
    (void)fprintf(output.fp, " = { 0, 0, 0 }"), Putl(output, 0);
   Putchr(';', output),Putchr('\n', output);
  }
  if (use(doutput)) {
   if (tp->U.V13.tsubid == (struct S64 *)NIL)
    (void)fprintf(output.fp, "%s", xtern), Putl(output, 0);
   (void)fprintf(output.fp, "text%c", tab1), Putl(output, 0);
   printid(defnams.A[(int)(doutput)]->U.V6.lid);
   if (tp->U.V13.tsubid != (struct S64 *)NIL)
    (void)fprintf(output.fp, " = { 0, 0, 0 }"), Putl(output, 0);
   Putchr(';', output),Putchr('\n', output);
  }
 }
 if (use(dinput) || use(dget) || use(dread) || use(dreadln) || use(deof) || use(deoln) || use(dreset) || use(drewrite)) {
  (void)fprintf(output.fp, "%sFread(x, f) fread((char *)&x, sizeof(x), 1, f)\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%sGet(f) Fread((f).buf, (f).fp)\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%sGetx(f) (f).init = 1, (f).eoln = (((f).buf = fgetc((f).fp)) == %s) ? (((f).buf = %s), 1) : 0\n", C4_define, nlchr, spchr), Putl(output, 1);
  (void)fprintf(output.fp, "%sGetchr(f) (f).buf, Getx(f)\n", C4_define), Putl(output, 1);
 }
 if (use(dread) || use(dreadln)) {
  (void)fprintf(output.fp, "%sFILE%c*Tmpfil;\n", C50_static, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%slong%cTmplng;\n", C50_static, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sdouble%cTmpdbl;\n", C50_static, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sFscan(f) (f).init ? ungetc((f).buf, (f).fp) : 0, Tmpfil = (f).fp\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%sScan(p, a) Scanck(fscanf(Tmpfil, p, a))\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%s%cScanck();\n", voidtyp, tab1), Putl(output, 1);
  if (use(dreadln))
   (void)fprintf(output.fp, "%s%cGetl();\n", voidtyp, tab1), Putl(output, 1);
 }
 if (use(deoln))
  (void)fprintf(output.fp, "%sEoln(f) ((f).eoln ? true : false)\n", C4_define), Putl(output, 1);
 if (use(deof))
  (void)fprintf(output.fp, "%sEof(f) ((((f).init == 0) ? (Get(f)) : 0, ((f).eof ? 1 : feof((f).fp))) ? true : false)\n", C4_define), Putl(output, 1);
 if (use(doutput) || use(dput) || use(dwrite) || use(dwriteln) || use(dreset) || use(drewrite) || use(dclose)) {
  (void)fprintf(output.fp, "%sFwrite(x, f) fwrite((char *)&x, sizeof(x), 1, f)\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%sPut(f) Fwrite((f).buf, (f).fp)\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%sPutx(f) (f).eoln = ((f).buf == %s), %sfputc((f).buf, (f).fp)\n", C4_define, nlchr, voidcast), Putl(output, 1);
  (void)fprintf(output.fp, "%sPutchr(c, f) (f).buf = (c), Putx(f)\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%sPutl(f, v) (f).eoln = v\n", C4_define), Putl(output, 1);
 }
 if (use(dreset) || use(drewrite) || use(dclose)) {
  (void)fprintf(output.fp, "%sFinish(f) ((f).out && !(f).eoln) ? (Putchr(%s, f), 0) : 0, !fseek((f).fp, 0L, 0)\n", C4_define, nlchr), Putl(output, 1);
  (void)fprintf(output.fp, "%sint%cfseek();\n", xtern, tab1), Putl(output, 1);
 }
 if (use(dclose)) {
  (void)fprintf(output.fp, "%sClose(f) (f).init = ((f).init ? (fclose((f).fp), 0) : 0), (f).fp = NULL\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%sClosex(f) (f).init = ((f).init ? (Finish(f), fclose((f).fp), 0) : 0), (f).fp = NULL\n", C4_define), Putl(output, 1);
 }
 if (use(dreset)) {
  (void)fprintf(output.fp, "%sREADONLY\n", ifdef), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%cRmode[] = \"r\";\n", C50_static, chartyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s\n", elsif), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%cRmode[] = \"r+\";\n", C50_static, chartyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s\n", endif), Putl(output, 1);
  (void)fprintf(output.fp, "%sReset(f, n, l) (f).init = (f).init ? !fseek((f).fp, 0L, 0) : (((f).fp = Fopen(n, l, Rmode)), 1), (f).eof = (f).out = 0, Get(f)\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%sResetx(f, n, l) (f).init = (f).init ? (Finish(f)) : (((f).fp = Fopen(n, l, Rmode)), 1), (f).eof = (f).out = 0, Getx(f)\n", C4_define), Putl(output, 1);
  usefopn = true;
 }
 if (use(drewrite)) {
  (void)fprintf(output.fp, "%sWRITEONLY\n", ifdef), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%cWmode[] = \"w\";\n", C50_static, chartyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s\n", elsif), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%cWmode[] = \"w+\";\n", C50_static, chartyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s\n", endif), Putl(output, 1);
  (void)fprintf(output.fp, "%sRewrite(f, n, l) (f).init = (f).init ? !fseek((f).fp, 0L, 0) : (((f).fp = Fopen(n, l, Wmode)), 1), (f).out = (f).eof = 1\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%sRewritex(f, n, l) (f).init = (f).init ? (Finish(f)) : (((f).fp = Fopen(n, l, Wmode)), 1), (f).out = (f).eof = (f).eoln = 1\n", C4_define), Putl(output, 1);
  usefopn = true;
 }
 if (usefopn) {
  (void)fprintf(output.fp, "FILE   *Fopen();\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s\n", ifndef, maxfilename), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s %1d\n", C4_define, maxfilename, (maxtoknlen + 1)), Putl(output, 1);
  (void)fprintf(output.fp, "%s\n", endif), Putl(output, 1);
 }
 if (usecase || usejmps) {
  (void)fprintf(output.fp, "/*\n"), Putl(output, 1);
  (void)fprintf(output.fp, "**     Definitions for case-statements\n"), Putl(output, 1);
  (void)fprintf(output.fp, "**     and for non-local gotos\n"), Putl(output, 1);
  (void)fprintf(output.fp, "*/\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%sLine __LINE__\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%cCaseerror();\n", C50_static, voidtyp, tab1), Putl(output, 1);
 }
 if (usejmps) {
  (void)fprintf(output.fp, "%s<setjmp.h>\n", C24_include), Putl(output, 1);
  (void)fprintf(output.fp, "%sstruct Jb { jmp_buf%cjb; } J[%1d];\n", C50_static, tab1, (maxlevel + 1)), Putl(output, 1);
 }
 if (use(dinteger) || use(dmaxint) || use(dboolean) || use(dfalse) || use(dtrue) || use(deof) || use(deoln) || use(dexp) || use(dln) || use(dsqr) || use(dsin) || use(dcos) || use(dtan) || use(darctan) || use(dsqrt) || use(dreal)) {
  (void)fprintf(output.fp, "/*\n"), Putl(output, 1);
  (void)fprintf(output.fp, "**     Definitions for standard types\n"), Putl(output, 1);
  (void)fprintf(output.fp, "*/\n"), Putl(output, 1);
 }
 if (usecomp) {
  (void)fprintf(output.fp, "%s%s strncmp();\n", xtern, inttyp), Putl(output, 1);
  (void)fprintf(output.fp, "%sCmpstr(x, y) strncmp((x), (y), sizeof(x))\n", C4_define), Putl(output, 1);
 }
 if (use(dboolean) || use(dfalse) || use(dtrue) || use(deof) || use(deoln) || usesets) {
  capital(defnams.A[(int)(dboolean)]);
  (void)fprintf(output.fp, "%s%s%c", typdef, chartyp, tab1), Putl(output, 0);
  printid(defnams.A[(int)(dboolean)]->U.V6.lid);
  Putchr(';', output),Putchr('\n', output);
  capital(defnams.A[(int)(dfalse)]);
  (void)fprintf(output.fp, "%s", C4_define), Putl(output, 0);
  printid(defnams.A[(int)(dfalse)]->U.V6.lid);
  (void)fprintf(output.fp, " ("), Putl(output, 0);
  printid(defnams.A[(int)(dboolean)]->U.V6.lid);
  (void)fprintf(output.fp, ")0\n"), Putl(output, 1);
  capital(defnams.A[(int)(dtrue)]);
  (void)fprintf(output.fp, "%s", C4_define), Putl(output, 0);
  printid(defnams.A[(int)(dtrue)]->U.V6.lid);
  (void)fprintf(output.fp, " ("), Putl(output, 0);
  printid(defnams.A[(int)(dboolean)]->U.V6.lid);
  (void)fprintf(output.fp, ")1\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%c*Bools[];\n", C50_static, chartyp, tab1), Putl(output, 1);
 }
 capital(defnams.A[(int)(dinteger)]);
 if (use(dinteger)) {
  (void)fprintf(output.fp, "%s%s%c", typdef, inttyp, tab1), Putl(output, 0);
  printid(defnams.A[(int)(dinteger)]->U.V6.lid);
  Putchr(';', output),Putchr('\n', output);
 }
 if (use(dmaxint))
  (void)fprintf(output.fp, "%smaxint%c%1d\n", C4_define, tab1, maxint), Putl(output, 1);
 capital(defnams.A[(int)(dreal)]);
 if (use(dreal)) {
  (void)fprintf(output.fp, "%s%s%c", typdef, realtyp, tab1), Putl(output, 0);
  printid(defnams.A[(int)(dreal)]->U.V6.lid);
  Putchr(';', output),Putchr('\n', output);
 }
 if (use(dexp))
  (void)fprintf(output.fp, "%s%s exp();\n", xtern, doubletyp), Putl(output, 1);
 if (use(dln))
  (void)fprintf(output.fp, "%s%s log();\n", xtern, doubletyp), Putl(output, 1);
 if (use(dsqr))
  (void)fprintf(output.fp, "%s%s pow();\n", xtern, doubletyp), Putl(output, 1);
 if (use(dsin))
  (void)fprintf(output.fp, "%s%s sin();\n", xtern, doubletyp), Putl(output, 1);
 if (use(dcos))
  (void)fprintf(output.fp, "%s%s cos();\n", xtern, doubletyp), Putl(output, 1);
 if (use(dtan))
  (void)fprintf(output.fp, "%s%s tan();\n", xtern, doubletyp), Putl(output, 1);
 if (use(darctan))
  (void)fprintf(output.fp, "%s%s atan();\n", xtern, doubletyp), Putl(output, 1);
 if (use(dsqrt))
  (void)fprintf(output.fp, "%s%s sqrt();\n", xtern, doubletyp), Putl(output, 1);
 if (use(dabs) && use(dreal))
  (void)fprintf(output.fp, "%s%s fabs();\n", xtern, doubletyp), Putl(output, 1);
 if (use(dnew) || usenilp) {
  (void)fprintf(output.fp, "/*\n"), Putl(output, 1);
  (void)fprintf(output.fp, "**     Definitions for pointers\n"), Putl(output, 1);
  (void)fprintf(output.fp, "*/\n"), Putl(output, 1);
 }
 if (use(dnew)) {
  (void)fprintf(output.fp, "%sUnionoffs\n", ifndef), Putl(output, 1);
  (void)fprintf(output.fp, "%sUnionoffs(p, m) (((long)(&(p)->m))-((long)(p)))\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%s\n", endif), Putl(output, 1);
 }
 if (usenilp)
  (void)fprintf(output.fp, "%sNIL 0\n", C4_define), Putl(output, 1);
 if ((tp->U.V13.tsubid == (struct S64 *)NIL) && (use(dnew) || use(ddispose) || use(dhalt) || use(dexit)))
  (void)fprintf(output.fp, "%s<stdlib.h>\n", C24_include), Putl(output, 1);
 if (usesets) {
  (void)fprintf(output.fp, "/*\n"), Putl(output, 1);
  (void)fprintf(output.fp, "**     Definitions for set-operations\n"), Putl(output, 1);
  (void)fprintf(output.fp, "*/\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%sClaimset() %sCurrset(0, (%s)0)\n", C4_define, voidcast, setptyp), Putl(output, 1);
  (void)fprintf(output.fp, "%sNewset() Currset(1, (%s)0)\n", C4_define, setptyp), Putl(output, 1);
  (void)fprintf(output.fp, "%sSaveset(s) Currset(2, s)\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%ssetbits %1d\n", C4_define, C37_setbits), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%c%s;\n", typdef, wordtype, tab1, setwtyp), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s *%c%s;\n", typdef, setwtyp, tab1, setptyp), Putl(output, 1);
  (void)fprintf(output.fp, "%s", C50_static), Putl(output, 0);
  printid(defnams.A[(int)(dboolean)]->U.V6.lid);
  (void)fprintf(output.fp, "%cMember(), Le(), Ge(), Eq(), Ne();\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%cUnion(), Diff();\n", C50_static, setptyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%cInsmem(), Mksubr();\n", C50_static, setptyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%cCurrset(), Inter();\n", C50_static, setptyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%cTmpset;\n", C50_static, setptyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%cConset[];\n", C50_static, setptyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%cSetncpy();\n", C50_static, voidtyp, tab1), Putl(output, 1);
 }
 if (align) {
  (void)fprintf(output.fp, "%sSETALIGN\n", ifndef), Putl(output, 1);
  (void)fprintf(output.fp, "%sSETALIGN(x) Alignset(x)\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%s", C50_static), Putl(output, 0);
  (void)fprintf(output.fp, "struct Set { %s%cS[%1d+1]; } *Alignset();\n", wordtype, tab1, maxsetrange), Putl(output, 1);
  (void)fprintf(output.fp, "%s\n", endif), Putl(output, 1);
  (void)fprintf(output.fp, "%sSTRALIGN\n", ifndef), Putl(output, 1);
  (void)fprintf(output.fp, "%sSTRALIGN(x) Alignstr(x)\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%s", C50_static), Putl(output, 0);
  (void)fprintf(output.fp, "struct String { char   A[%1d+1]; } *Alignstr();\n", maxtoknlen), Putl(output, 1);
  (void)fprintf(output.fp, "%s\n", endif), Putl(output, 1);
 }
 (void)fprintf(output.fp, "%s%s *strncpy();\n", xtern, chartyp), Putl(output, 1);
 if (use(dargc) || use(dargv)) {
  (void)fprintf(output.fp, "/*\n"), Putl(output, 1);
  (void)fprintf(output.fp, "**     Definitions for argv-operations\n"), Putl(output, 1);
  (void)fprintf(output.fp, "*/\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%s%cargc;\n", inttyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s%c**argv;\n", chartyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, " void\n"), Putl(output, 1);
  (void)fprintf(output.fp, "Argvgt(n, cp, l)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%s%cn;\n", inttyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%cl;\n", registr, inttyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s%c*cp;\n", registr, chartyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%s%s%c*sp;\n", tab1, registr, chartyp, tab1), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%cfor (sp = argv[n]; l > 0 && *sp; l--)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s*cp++ = *sp++;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%cwhile (l-- > 0)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s*cp++ = %s;\n", tab2, spchr), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if ((tp->U.V13.tsubconst != (struct S64 *)NIL) || (tp->U.V13.tsubtype != (struct S64 *)NIL) || (tp->U.V13.tsubvar != (struct S64 *)NIL) || (tp->U.V13.tsubsub != (struct S64 *)NIL)) {
  (void)fprintf(output.fp, "/*\n"), Putl(output, 1);
  (void)fprintf(output.fp, "**     Start of program definitions\n"), Putl(output, 1);
  (void)fprintf(output.fp, "*/\n"), Putl(output, 1);
 }
 econst(tp->U.V13.tsubconst);
 etype(tp->U.V13.tsubtype);
 evar(tp->U.V13.tsubvar);
 if (tp->U.V13.tsubsub != (struct S64 *)NIL)
  Putchr('\n', output);
 esubr(tp->U.V13.tsubsub);
 if (tp->U.V13.tsubid != (struct S64 *)NIL) {
  (void)fprintf(output.fp, "/*\n"), Putl(output, 1);
  (void)fprintf(output.fp, "**     Start of program code\n"), Putl(output, 1);
  (void)fprintf(output.fp, "*/\n"), Putl(output, 1);
  if (use(dargc) || use(dargv)) {
   (void)fprintf(output.fp, "main(_ac, _av)\n"), Putl(output, 1);
   (void)fprintf(output.fp, "%s%c_ac;\n", inttyp, tab1), Putl(output, 1);
   (void)fprintf(output.fp, "%s%c*_av[];\n", chartyp, tab1), Putl(output, 1);
   Putchr('{', output),Putchr('\n', output);
   Putchr('\n', output);
   (void)fprintf(output.fp, "%cargc = _ac;\n", tab1), Putl(output, 1);
   (void)fprintf(output.fp, "%cargv = _av;\n", tab1), Putl(output, 1);
  } else {
   (void)fprintf(output.fp, "main()\n"), Putl(output, 1);
   Putchr('{', output),Putchr('\n', output);
  }
  if (use(dinput))
   (void)fprintf(output.fp, "%cinput.fp = stdin;\n", tab1), Putl(output, 1);
  if (use(doutput))
   (void)fprintf(output.fp, "%coutput.fp = stdout;\n", tab1), Putl(output, 1);
  if (use(dinput)) {
   (void)fprintf(output.fp, "%sSTDINIT\n", ifdef), Putl(output, 1);
   (void)fprintf(output.fp, "%c%s(Getx(input));\n", tab1, voidcast), Putl(output, 1);
   (void)fprintf(output.fp, "%s\n", endif), Putl(output, 1);
  }
  increment();
  elabel(tp);
  estmt(tp->U.V13.tsubstmt);
  indent();
  (void)fprintf(output.fp, "exit(0);\n"), Putl(output, 1);
  indent();
  (void)fprintf(output.fp, "/* NOTREACHED */\n"), Putl(output, 1);
  decrement();
  Putchr('}', output),Putchr('\n', output);
  edconst(tp->U.V13.tsubconst);
  (void)fprintf(output.fp, "/*\n"), Putl(output, 1);
  (void)fprintf(output.fp, "**     End of program code\n"), Putl(output, 1);
  (void)fprintf(output.fp, "*/\n"), Putl(output, 1);
 }
}

void econset();

 integer
size(tp)
 treeptr tp;
{
 register integer R201;
 integer r, x;

 r = 0;
 while (tp != (struct S64 *)NIL) {
  if (tp->tt == nrange)
   x = cvalof(tp->U.V41.texpr);
  else
   if (tp->tt == nempty)
    x = 0;
   else
    x = cvalof(tp);
  if (x > r)
   r = x;
  tp = tp->tnext;
 }
 R201 = csetwords(r + 1);
 return R201;
}

void ebits();

 void
eword(s)
 bitset s;
{
# define bitshex 4
 integer n;
 register integer i;
 unsigned char x;

 n = 0;
 while (n <= C37_setbits)
  n = n + bitshex;
 n = n - bitshex;
 while (n >= 0) {
  x = 0;
  {
   integer B57 = 0,
    B58 = bitshex - 1;

   if (B57 <= B58)
    for (i = B57; ; i++) {
     if (Member((unsigned)((n + i)), s.S))
      switch ((int)(i)) {
        case 0:
       x = x + 1;
       break ;
        case 1:
       x = x + 2;
       break ;
        case 2:
       x = x + 4;
       break ;
        case 3:
       x = x + 8;
       break ;
        default:
       Caseerror(Line);
      }
     if (i == B58) break;
    }
  }
  Putchr(hexdig.A[x], output);
  n = n - bitshex;
 }
# undef bitshex
}

 void
ebits(tp)
 treeptr tp;
{
 typedef struct { bitset A[maxsetrange + 1]; } T77;
 T77 sets;
 integer s;
 register integer m;
 register integer n;

 s = size(tp);
 {
  integer B59 = 0,
   B60 = s - 1;

  if (B59 <= B60)
   for (n = B59; ; n++) {
    Setncpy(sets.A[n].S, Conset[182], sizeof(sets.A[n].S));
    if (n == B60) break;
   }
 }
 while (tp != (struct S64 *)NIL) {
  if (tp->tt == nrange)
   {
    integer B61 = cvalof(tp->U.V41.texpl),
     B62 = cvalof(tp->U.V41.texpr);

    if (B61 <= B62)
     for (m = B61; ; m++) {
      n = m / (C37_setbits + 1);
      Setncpy(sets.A[n].S, Union(sets.A[n].S, Saveset((Tmpset = Newset(), (void)Insmem((unsigned)(m % (C37_setbits + 1)), Tmpset), Tmpset))), sizeof(sets.A[n].S));
      Claimset();
      if (m == B62) break;
     }
   }
  else
   if (tp->tt != nempty) {
    m = cvalof(tp);
    n = m / (C37_setbits + 1);
    Setncpy(sets.A[n].S, Union(sets.A[n].S, Saveset((Tmpset = Newset(), (void)Insmem((unsigned)(m % (C37_setbits + 1)), Tmpset), Tmpset))), sizeof(sets.A[n].S));
    Claimset();
   }
  tp = tp->tnext;
 }
 (void)fprintf(output.fp, "%c%1d", tab1, s), Putl(output, 0);
 {
  integer B63 = 0,
   B64 = s - 1;

  if (B63 <= B64)
   for (n = B63; ; n++) {
    Putchr(',', output);
    if (n % 6 == 0)
     Putchr('\n', output);
    (void)fprintf(output.fp, "%c0x", tab1), Putl(output, 0);
    eword(sets.A[n]);
    if (n == B64) break;
   }
 }
 Putchr('\n', output);
}

 void
econset(tp, len)
 treeptr tp;
 integer len;
{
 register integer i;

 i = 0;
 while (tp != (struct S64 *)NIL) {
  (void)fprintf(output.fp, "%s%s%cQ%1d[] = {\n", C50_static, setwtyp, tab1, i), Putl(output, 1);
  ebits(tp->U.V42.texps);
  (void)fprintf(output.fp, "};\n"), Putl(output, 1);
  i = i + 1;
  tp = tp->tnext;
 }
 (void)fprintf(output.fp, "%s%s%c*Conset[] = {\n", C50_static, setwtyp, tab1), Putl(output, 1);
 {
  integer B65 = len - 1,
   B66 = 1;

  if (B65 >= B66)
   for (i = B65; ; i--) {
    (void)fprintf(output.fp, "%cQ%1d,", tab1, i), Putl(output, 0);
    if (i % 6 == 5)
     Putchr('\n', output);
    if (i == B66) break;
   }
 }
 (void)fprintf(output.fp, "%cQ0\n", tab1), Putl(output, 1);
 (void)fprintf(output.fp, "};\n"), Putl(output, 1);
}

 void
emit()
{
 static char usigned[]       = "unsigned ";
 boolean conflag, setused, dropset, doarrow, donearr;
 integer indnt;
 boolean *F221;
 boolean *F223;
 boolean *F225;
 boolean *F227;
 boolean *F229;
 integer *F231;

 F231 = G230_indnt;
 G230_indnt = &indnt;
 F229 = G228_donearr;
 G228_donearr = &donearr;
 F227 = G226_doarrow;
 G226_doarrow = &doarrow;
 F225 = G224_dropset;
 G224_dropset = &dropset;
 F223 = G222_setused;
 G222_setused = &setused;
 F221 = G220_conflag;
 G220_conflag = &conflag;
 (*G230_indnt) = 0;
 varno = 0;
 (*G220_conflag) = false;
 (*G222_setused) = false;
 (*G224_dropset) = false;
 (*G226_doarrow) = false;
 (*G228_donearr) = false;
 eprogram(top);
 if (usebool)
  (void)fprintf(output.fp, "%s%s%c*Bools[] = { \"false\", \"true\" };\n", C50_static, chartyp, tab1), Putl(output, 1);
 if (usescan) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s%s\n", C50_static, voidtyp), Putl(output, 1);
  (void)fprintf(output.fp, "Scanck(n)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%s%cn;\n", inttyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%cif (n != 1) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s%sfprintf(stderr, \"Bad input\\n\");\n", tab2, voidcast), Putl(output, 1);
  (void)fprintf(output.fp, "%sexit(1);\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usegetl) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s%s\n", C50_static, voidtyp), Putl(output, 1);
  (void)fprintf(output.fp, "Getl(f)\n"), Putl(output, 1);
  (void)fprintf(output.fp, " text%c*f;\n", tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%cwhile (f->eoln == 0)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sGetx(*f);\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%cGetx(*f);\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usefopn) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%sFILE *\n", C50_static), Putl(output, 1);
  (void)fprintf(output.fp, "Fopen(n, l, m)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%s%c*n, *m;\n", chartyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s%cl;\n", inttyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%cFILE%s*f;\n", tab1, tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%c*s;\n", tab1, registr, chartyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%cch = %cA%c;\n", tab1, C50_static, chartyp, tab1, quote, quote), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%ctmp[%s];\n", tab1, C50_static, chartyp, tab1, maxfilename), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%cunlink(),\n", tab1, xtern, inttyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sstrlen();\n", tab3), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%cif (n == NULL)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%ssprintf(tmp, %sch++);\n", tab2, tmpfilename), Putl(output, 1);
  (void)fprintf(output.fp, "%celse {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sif (l < 0)\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sl = strlen(n);\n", tab3), Putl(output, 1);
  (void)fprintf(output.fp, "%sstrncpy(tmp, n, sizeof(tmp));\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sfor (s = &tmp[sizeof(tmp)-1]; *s == %s || *s == %s || s - tmp > l; )\n", tab2, spchr, nulchr), Putl(output, 1);
  (void)fprintf(output.fp, "%s*s-- = %s;\n", tab3, nulchr), Putl(output, 1);
  (void)fprintf(output.fp, "%sif (tmp[sizeof(tmp)-1]) {\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%s%sfprintf(stderr, \"Too long filename %c%%s%c\\n\", n);\n", tab3, voidcast, quote, quote), Putl(output, 1);
  (void)fprintf(output.fp, "%sexit(1);\n", tab3), Putl(output, 1);
  (void)fprintf(output.fp, "%s}\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cs = tmp;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cif ((f = fopen(s, m)) == NULL) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s%sfprintf(stderr, \"Cannot open: %%s\\n\", s);\n", tab2, voidcast), Putl(output, 1);
  (void)fprintf(output.fp, "%sexit(1);\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cif (n == NULL)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sunlink(tmp);\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn (f);\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (setcnt > 0)
  econset(setlst, setcnt);
 if (useunion) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s%s\n", C50_static, setptyp), Putl(output, 1);
  (void)fprintf(output.fp, "Union(p1, p2)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%cp1, p2;\n", tab1, registr, setptyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%s%s%si, j, k;\n", tab1, registr, inttyp, tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%ssp = Newset(),\n", tab1, registr, setptyp, tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sp3 = sp;\n", tab4), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%cj = *p1;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c*p3 = j;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cif (j > *p2)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sj = *p2;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%celse\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s*p3 = *p2;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%ck = *p1 - *p2;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cp1++, p2++, p3++;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cfor (i = 0; i < j; i++)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s*p3++ = (*p1++ | *p2++);\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%cwhile (k > 0) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s*p3++ = *p1++;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sk--;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cwhile (k < 0) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s*p3++ = *p2++;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sk++;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn (Saveset(sp));\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usediff) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s%s\n", C50_static, setptyp), Putl(output, 1);
  (void)fprintf(output.fp, "Diff(p1, p2)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%cp1, p2;\n", tab1, registr, setptyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%s%s%si, j, k;\n", tab1, registr, inttyp, tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%ssp = Newset(),\n", tab1, registr, setptyp, tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sp3 = sp;\n", tab4), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%cj = *p1;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c*p3 = j;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cif (j > *p2)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sj = *p2;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%ck = *p1 - *p2;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cp1++, p2++, p3++;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cfor (i = 0; i < j; i++)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s*p3++ = (*p1++ & ~ (*p2++));\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%cwhile (k > 0) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s*p3++ = *p1++;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sk--;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn (Saveset(sp));\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (useintr) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s%s\n", C50_static, setptyp), Putl(output, 1);
  (void)fprintf(output.fp, "Inter(p1, p2)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%cp1, p2;\n", tab1, registr, setptyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%s%s%si, j, k;\n", tab1, registr, inttyp, tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%ssp = Newset(),\n", tab1, registr, setptyp, tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sp3 = sp;\n", tab4), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%cif ((j = *p1) > *p2)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sj = *p2;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c*p3 = j;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cp1++, p2++, p3++;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cfor (i = 0; i < j; i++)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s*p3++ = (*p1++ & *p2++);\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn (Saveset(sp));\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usememb) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s", C50_static), Putl(output, 0);
  printid(defnams.A[(int)(dboolean)]->U.V6.lid);
  Putchr('\n', output);
  (void)fprintf(output.fp, "Member(m, sp)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%s%cm;\n", tab1, registr, usigned, inttyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%csp;\n", tab1, registr, setptyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%s%s%s%ci = m / (setbits+1) + 1;\n", tab1, registr, usigned, inttyp, tab1), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%cif ((i <= *sp) && (sp[i] & (1 << (m %% (setbits+1)))))\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sreturn (", tab2), Putl(output, 0);
  printid(defnams.A[(int)(dtrue)]->U.V6.lid);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn (", tab1), Putl(output, 0);
  printid(defnams.A[(int)(dfalse)]->U.V6.lid);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (useseq || usesne) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s", C50_static), Putl(output, 0);
  printid(defnams.A[(int)(dboolean)]->U.V6.lid);
  Putchr('\n', output);
  (void)fprintf(output.fp, "Eq(p1, p2)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%cp1, p2;\n", tab1, registr, setptyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%s%s%ci, j;\n", tab1, registr, inttyp, tab1), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%ci = *p1++;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cj = *p2++;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cwhile (i != 0 && j != 0) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sif (*p1++ != *p2++)\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sreturn (", tab3), Putl(output, 0);
  printid(defnams.A[(int)(dfalse)]->U.V6.lid);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%si--, j--;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cwhile (i != 0) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sif (*p1++ != 0)\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sreturn (", tab3), Putl(output, 0);
  printid(defnams.A[(int)(dfalse)]->U.V6.lid);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%si--;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cwhile (j != 0) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sif (*p2++ != 0)\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sreturn (", tab3), Putl(output, 0);
  printid(defnams.A[(int)(dfalse)]->U.V6.lid);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%sj--;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn (", tab1), Putl(output, 0);
  printid(defnams.A[(int)(dtrue)]->U.V6.lid);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usesne) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s", C50_static), Putl(output, 0);
  printid(defnams.A[(int)(dboolean)]->U.V6.lid);
  Putchr('\n', output);
  (void)fprintf(output.fp, "Ne(p1, p2)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%cp1, p2;\n", tab1, registr, setptyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%creturn (!Eq(p1, p2));", tab1), Putl(output, 0);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usesle) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s", C50_static), Putl(output, 0);
  printid(defnams.A[(int)(dboolean)]->U.V6.lid);
  Putchr('\n', output);
  (void)fprintf(output.fp, "Le(p1, p2)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%cp1, p2;\n", tab1, registr, setptyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%s%s%ci, j;\n", tab1, registr, inttyp, tab1), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%ci = *p1++;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cj = *p2++;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cwhile (i != 0 && j != 0) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sif ((*p1++ & ~ *p2++) != 0)\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sreturn (", tab3), Putl(output, 0);
  printid(defnams.A[(int)(dfalse)]->U.V6.lid);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%si--, j--;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cwhile (i != 0) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sif (*p1++ != 0)\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sreturn (", tab3), Putl(output, 0);
  printid(defnams.A[(int)(dfalse)]->U.V6.lid);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%si--;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn (", tab1), Putl(output, 0);
  printid(defnams.A[(int)(dtrue)]->U.V6.lid);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usesge) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s", C50_static), Putl(output, 0);
  printid(defnams.A[(int)(dboolean)]->U.V6.lid);
  Putchr('\n', output);
  (void)fprintf(output.fp, "Ge(p1, p2)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%cp1, p2;\n", tab1, registr, setptyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%s%s%ci, j;\n", tab1, registr, inttyp, tab1), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%ci = *p1++;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cj = *p2++;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cwhile (i != 0 && j != 0) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sif ((*p2++ & ~ *p1++) != 0)\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sreturn (false);\n", tab3), Putl(output, 1);
  (void)fprintf(output.fp, "%si--, j--;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cwhile (j != 0) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sif (*p2++ != 0)\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sreturn (", tab3), Putl(output, 0);
  printid(defnams.A[(int)(dfalse)]->U.V6.lid);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%sj--;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn (", tab1), Putl(output, 0);
  printid(defnams.A[(int)(dtrue)]->U.V6.lid);
  (void)fprintf(output.fp, ");\n"), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usemksub) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s%s\n", C50_static, setptyp), Putl(output, 1);
  (void)fprintf(output.fp, "Mksubr(lo, hi, sp)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%s%clo, hi;\n", tab1, registr, usigned, inttyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%csp;\n", tab1, registr, setptyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%s%s%ci, k;\n", tab1, registr, inttyp, tab1), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%cif (hi < lo)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sreturn (sp);\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%ci = hi / (setbits+1) + 1;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cfor (k = *sp + 1; k <= i; k++)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%ssp[k] = 0;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%cif (*sp < i)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s*sp = i;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%cfor (k = lo; k <= hi; k++)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%ssp[k / (setbits+1) + 1] |= (1 << (k %% (setbits+1)));\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn (sp);\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (useins) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s%s\n", C50_static, setptyp), Putl(output, 1);
  (void)fprintf(output.fp, "Insmem(m, sp)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%s%cm;\n", tab1, registr, usigned, inttyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%csp;\n", tab1, registr, setptyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%s%s%ci,\n", tab1, registr, inttyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s%cj = m / (setbits+1) + 1;\n", tab3, tab1), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%cif (*sp < j)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sfor (i = *sp + 1, *sp = j; i <= *sp; i++)\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%ssp[i] = 0;\n", tab3), Putl(output, 1);
  (void)fprintf(output.fp, "%csp[j] |= (1 << (m %% (setbits+1)));\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn (sp);\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usesets) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%sSETSPACE\n", ifndef), Putl(output, 1);
  (void)fprintf(output.fp, "%sSETSPACE 256\n", C4_define), Putl(output, 1);
  (void)fprintf(output.fp, "%s\n", endif), Putl(output, 1);
  (void)fprintf(output.fp, "%s%s\n", C50_static, setptyp), Putl(output, 1);
  (void)fprintf(output.fp, "Currset(n,sp)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%cn;\n", tab1, inttyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%csp;\n", tab1, setptyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%s%s%cSpace[SETSPACE];\n", tab1, C50_static, setwtyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%cTop = Space;\n", tab1, C50_static, setptyp, tab1), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%cswitch (n) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c  case 0:\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sTop = Space;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sreturn (0);\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c  case 1:\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sif (&Space[SETSPACE] - Top <= %1d) {\n", tab2, maxsetrange), Putl(output, 1);
  (void)fprintf(output.fp, "%s%sfprintf(stderr, \"Set-space exhausted\\n\");\n", tab3, voidcast), Putl(output, 1);
  (void)fprintf(output.fp, "%sexit(1);\n", tab3), Putl(output, 1);
  (void)fprintf(output.fp, "%s}\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%s*Top = 0;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sreturn (Top);\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c  case 2:\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sif (Top <= &sp[*sp])\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%sTop = &sp[*sp + 1];\n", tab3), Putl(output, 1);
  (void)fprintf(output.fp, "%sreturn (sp);\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c/* NOTREACHED */\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usescpy) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s%s\n", C50_static, voidtyp), Putl(output, 1);
  (void)fprintf(output.fp, "Setncpy(S1, S2, N)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%cS1, S2;\n", tab1, registr, setptyp, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%s%cN;\n", tab1, registr, usigned, inttyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%s%s%s%cm;\n", tab1, registr, usigned, inttyp, tab1), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%cN /= sizeof(%s);\n", tab1, setwtyp), Putl(output, 1);
  (void)fprintf(output.fp, "%c*S1++ = --N;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cm = *S2++;\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cwhile (m != 0 && N != 0) {\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s*S1++ = *S2++;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%s--N;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%s--m;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%c}\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%cwhile (N-- != 0)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s*S1++ = 0;\n", tab2), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usesal) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%sstruct Set *\n", C50_static), Putl(output, 1);
  (void)fprintf(output.fp, "Alignset(Sp)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%c*Sp;\n", tab1, registr, wordtype, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%sstruct Set%ctmp;\n", tab1, C50_static, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%c*tp = tmp.S;\n", tab1, registr, wordtype, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%si = *Sp;\n", tab1, registr, inttyp, tab2), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%cwhile (i-- >= 0)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s*tp++ = *Sp++;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn (&tmp);\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usealig) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%sstruct String *\n", C50_static), Putl(output, 1);
  (void)fprintf(output.fp, "Alignstr(Cp)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%c*Cp;\n", tab1, registr, chartyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%sstruct String%ctmp;\n", tab1, C50_static, tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%s%c*sp = tmp.A;\n", tab1, registr, chartyp, tab1), Putl(output, 1);
  Putchr('\n', output);
  (void)fprintf(output.fp, "%cwhile (*sp++ = *Cp++)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%s;\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn (&tmp);\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usecase || usejmps) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s%s\n", C50_static, voidtyp), Putl(output, 1);
  (void)fprintf(output.fp, "Caseerror(n)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%cn;\n", tab1, inttyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%sfprintf(stderr, \"Missing case limb: line %%d\\n\", n);\n", tab1, voidcast), Putl(output, 1);
  (void)fprintf(output.fp, "%cexit(1);\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%c/* NOTREACHED */\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (usemax) {
  Putchr('\n', output);
  (void)fprintf(output.fp, "%s%s\n", C50_static, inttyp), Putl(output, 1);
  (void)fprintf(output.fp, "Max(m, n)\n"), Putl(output, 1);
  (void)fprintf(output.fp, "%c%s%cm, n;\n", tab1, inttyp, tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%cif (m > n)\n", tab1), Putl(output, 1);
  (void)fprintf(output.fp, "%sreturn (m);\n", tab2), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn (n);\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (use(dtrunc)) {
  (void)fprintf(output.fp, "%s%s\n", C50_static, inttyp), Putl(output, 1);
  (void)fprintf(output.fp, "Trunc(f)\n"), Putl(output, 1);
  printid(defnams.A[(int)(dreal)]->U.V6.lid);
  (void)fprintf(output.fp, "%cf;\n", tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%creturn f;\n", tab1), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 if (use(dround)) {
  (void)fprintf(output.fp, "%s%s\n", C50_static, inttyp), Putl(output, 1);
  (void)fprintf(output.fp, "Round(f)\n"), Putl(output, 1);
  printid(defnams.A[(int)(dreal)]->U.V6.lid);
  (void)fprintf(output.fp, "%cf;\n", tab1), Putl(output, 1);
  Putchr('{', output),Putchr('\n', output);
  (void)fprintf(output.fp, "%c%s%s floor();\n", tab1, xtern, doubletyp), Putl(output, 1);
  (void)fprintf(output.fp, "%creturn floor(%s(0.5+f));\n", tab1, dblcast), Putl(output, 1);
  Putchr('}', output),Putchr('\n', output);
 }
 G220_conflag = F221;
 G222_setused = F223;
 G224_dropset = F225;
 G226_doarrow = F227;
 G228_donearr = F229;
 G230_indnt = F231;
}

void initialize();

 void
defname(cn, str)
 cnames cn;
 keyword str;
{
 toknbuf w;
 register toknidx i;

 {   register int _j, _i = 1 - 1;
     for (_j = 0; _j < 10; )
  w.A[_i++] = str.A[_j++];
 }
 {
  toknidx B67 = 1,
   B68 = keywordlen;

  if (B67 <= B68)
   for (i = B67; ; i++) {
    if (w.A[i - 1] == space) {
     w.A[i - 1] = null;
     goto L999;
    }
    if (i == B68) break;
   }
 }
 w.A[keywordlen + 1 - 1] = null;
L999:
 ctable.A[(int)(cn)] = saveid(&w);
}

 void
defid(nt, did, str)
 treetyp nt;
 predefs did;
 keyword str;
{
 toknbuf w;
 register toknidx i;
 treeptr tp, tq, tv;

 {
  toknidx B69 = 1,
   B70 = keywordlen;

  if (B69 <= B70)
   for (i = B69; ; i++) {
    if (str.A[i - 1] == space) {
     w.A[i - 1] = null;
     goto L999;
    } else
     w.A[i - 1] = str.A[i - 1];
    if (i == B70) break;
   }
 }
 w.A[keywordlen + 1 - 1] = null;
L999:
 tp = newid(saveid(&w));
 defnams.A[(int)(did)] = tp->U.V43.tsym;
 if (Member((unsigned)(nt), Conset[183])) {
  tv = mknode(npredef);
  tv->U.V12.tdef = did;
  tv->U.V12.tobtyp = tnone;
 } else
  tv = (struct S64 *)NIL;
 switch ((int)(nt)) {
   case nscalar:
  tv = mknode(nscalar);
  tv->U.V17.tscalid = (struct S64 *)NIL;
  tq = mknode(ntype);
  tq->U.V14.tbind = tv;
  tq->U.V14.tidl = tp;
  tp = tq;
  break ;
   case nconst:  case ntype:  case nfield:  case nvar:
  tq = mknode(nt);
  tq->U.V14.tbind = tv;
  tq->U.V14.tidl = tp;
  tq->U.V14.tattr = anone;
  tp = tq;
  break ;
   case nfunc:  case nproc:
  tq = mknode(nt);
  tq->U.V13.tsubid = tp;
  tq->U.V13.tsubstmt = tv;
  tq->U.V13.tfuntyp = (struct S64 *)NIL;
  tq->U.V13.tsubpar = (struct S64 *)NIL;
  tq->U.V13.tsublab = (struct S64 *)NIL;
  tq->U.V13.tsubconst = (struct S64 *)NIL;
  tq->U.V13.tsubtype = (struct S64 *)NIL;
  tq->U.V13.tsubvar = (struct S64 *)NIL;
  tq->U.V13.tsubsub = (struct S64 *)NIL;
  tq->U.V13.tscope = (struct S63 *)NIL;
  tq->U.V13.tstat = 0;
  tp = tq;
  break ;
   case nid:
  break ;
   default:
  Caseerror(Line);
 }
 deftab.A[(int)(did)] = tp;
}

 void
defkey(s, w)
 symtyp s;
 keyword w;
{
 register unsigned char i;

 {
  unsigned char B71 = 1,
   B72 = keywordlen;

  if (B71 <= B72)
   for (i = B71; ; i++) {
    if (w.A[i - 1] == space)
     w.A[i - 1] = null;
    if (i == B72) break;
   }
 }
 {
  register struct S232 *W73 = &keytab.A[(unsigned)(s)];

  W73->wrd = w;
  W73->sym = s;
 }
}

 void
fixinit(i)
 strindx i;
{
 toknbuf t;

 gettokn(i, &t);
 t.A[1 - 1] = 'i';
 puttokn(i, &t);
}

 void
defmach(lo, hi, str)
 integer lo, hi;
 machdefstr str;
{
 register toknidx i;
 toknbuf w;

 {   register int _j, _i = 1 - 1;
     for (_j = 0; _j < 16; )
  w.A[_i++] = str.A[_j++];
 }
 if (w.A[machdeflen - 1] != space)
  error(ebadmach);
 {
  toknidx B74 = machdeflen - 1,
   B75 = 1;

  if (B74 >= B75)
   for (i = B74; ; i--) {
    if (w.A[i - 1] != space) {
     w.A[i + 1 - 1] = null;
     goto L999;
    }
    if (i == B75) break;
   }
 }
 error(ebadmach);
L999:
 if (nmachdefs >= maxmachdefs)
  error(emanymachs);
 nmachdefs = nmachdefs + 1;
 {
  register struct S219 *W76 = &machdefs.A[nmachdefs - 1];

  W76->lolim = lo;
  W76->hilim = hi;
  W76->typstr = savestr(&w);
 }
}

 void
initstrstore()
{
 register strbcnt i;

 {
  strbcnt B77 = 1,
   B78 = maxblkcnt;

  if (B77 <= B78)
   for (i = B77; ; i++) {
    strstor.A[i] = (strblk *)NIL;
    if (i == B78) break;
   }
 }
 strstor.A[0] = (strblk *)malloc((unsigned)(sizeof(*strstor.A[0])));
 strstor.A[0]->A[0] = null;
 strfree = 1;
 strleft = maxstrblk;
}

 void
initialize()
{
 typedef struct { char A[16 - 1 + 1]; } T78;
 register hashtyp s;
 register pretyps t;
 register predefs d;
 T78 hx;

 lineno = 1;
 colno = 0;
 pushed = false;
 initstrstore();
 setlst = (struct S64 *)NIL;
 setcnt = 0;
 (void)strncpy(hx.A, "0123456789ABCDEF", sizeof(hx.A));
 {   register int _j, _i = 0 - 0;
     for (_j = 0; _j < 16; )
  hexdig.A[_i++] = hx.A[_j++];
 }
 symtab = (struct S63 *)NIL;
 statlvl = 0;
 maxlevel = -1;
 enterscope((declptr)NIL);
 varno = 0;
 usenilp = false;
 usesets = false;
 useunion = false;
 usediff = false;
 usemksub = false;
 useintr = false;
 usesge = false;
 usesle = false;
 usesne = false;
 useseq = false;
 usememb = false;
 useins = false;
 usescpy = false;
 usefopn = false;
 usescan = false;
 usegetl = false;
 usecase = false;
 usejmps = false;
 usebool = false;
 usecomp = false;
 usemax = false;
 usealig = false;
 usesal = false;
 {
  hashtyp B79 = 0,
   B80 = hashmax;

  if (B79 <= B80)
   for (s = B79; ; s++) {
    idtab.A[s] = (struct S62 *)NIL;
    if (s == B80) break;
   }
 }
 {
  predefs B81 = dabs,
   B82 = dztring;

  if ((int)(B81) <= (int)(B82))
   for (d = B81; ; d = (predefs)((int)(d)+1)) {
    deftab.A[(int)(d)] = (struct S64 *)NIL;
    defnams.A[(int)(d)] = (struct S65 *)NIL;
    if (d == B82) break;
   }
 }
 defkey(sand, *((keyword *)STRALIGN("and       ")));
 defkey(sarray, *((keyword *)STRALIGN("array     ")));
 defkey(sbegin, *((keyword *)STRALIGN("begin     ")));
 defkey(scase, *((keyword *)STRALIGN("case      ")));
 defkey(sconst, *((keyword *)STRALIGN("const     ")));
 defkey(sdiv, *((keyword *)STRALIGN("div       ")));
 defkey(sdo, *((keyword *)STRALIGN("do        ")));
 defkey(sdownto, *((keyword *)STRALIGN("downto    ")));
 defkey(selse, *((keyword *)STRALIGN("else      ")));
 defkey(send, *((keyword *)STRALIGN("end       ")));
 defkey(sextern, *((keyword *)STRALIGN(externsym)));
 defkey(sfile, *((keyword *)STRALIGN("file      ")));
 defkey(sfor, *((keyword *)STRALIGN("for       ")));
 defkey(sforward, *((keyword *)STRALIGN("forward   ")));
 defkey(sfunc, *((keyword *)STRALIGN("function  ")));
 defkey(sgoto, *((keyword *)STRALIGN("goto      ")));
 defkey(sif, *((keyword *)STRALIGN("if        ")));
 defkey(sinn, *((keyword *)STRALIGN("in        ")));
 defkey(slabel, *((keyword *)STRALIGN("label     ")));
 defkey(smod, *((keyword *)STRALIGN("mod       ")));
 defkey(snil, *((keyword *)STRALIGN("nil       ")));
 defkey(snot, *((keyword *)STRALIGN("not       ")));
 defkey(sof, *((keyword *)STRALIGN("of        ")));
 defkey(sor, *((keyword *)STRALIGN("or        ")));
 defkey(sother, *((keyword *)STRALIGN(othersym)));
 defkey(spacked, *((keyword *)STRALIGN("packed    ")));
 defkey(sproc, *((keyword *)STRALIGN("procedure ")));
 defkey(spgm, *((keyword *)STRALIGN("program   ")));
 defkey(srecord, *((keyword *)STRALIGN("record    ")));
 defkey(srepeat, *((keyword *)STRALIGN("repeat    ")));
 defkey(sset, *((keyword *)STRALIGN("set       ")));
 defkey(sthen, *((keyword *)STRALIGN("then      ")));
 defkey(sto, *((keyword *)STRALIGN("to        ")));
 defkey(stype, *((keyword *)STRALIGN("type      ")));
 defkey(suntil, *((keyword *)STRALIGN("until     ")));
 defkey(svar, *((keyword *)STRALIGN("var       ")));
 defkey(swhile, *((keyword *)STRALIGN("while     ")));
 defkey(swith, *((keyword *)STRALIGN("with      ")));
 defkey(seof, *((keyword *)STRALIGN(dummysym)));
 cprio.A[(int)(nformat) - (int)(nassign)] = 0;
 cprio.A[(int)(nrange) - (int)(nassign)] = 0;
 cprio.A[(int)(nin) - (int)(nassign)] = 0;
 cprio.A[(int)(nset) - (int)(nassign)] = 0;
 cprio.A[(int)(nassign) - (int)(nassign)] = 0;
 cprio.A[(int)(nor) - (int)(nassign)] = 1;
 cprio.A[(int)(nand) - (int)(nassign)] = 2;
 cprio.A[(int)(neq) - (int)(nassign)] = 3;
 cprio.A[(int)(nne) - (int)(nassign)] = 3;
 cprio.A[(int)(nlt) - (int)(nassign)] = 3;
 cprio.A[(int)(nle) - (int)(nassign)] = 3;
 cprio.A[(int)(ngt) - (int)(nassign)] = 3;
 cprio.A[(int)(nge) - (int)(nassign)] = 3;
 cprio.A[(int)(nplus) - (int)(nassign)] = 4;
 cprio.A[(int)(nminus) - (int)(nassign)] = 4;
 cprio.A[(int)(nmul) - (int)(nassign)] = 5;
 cprio.A[(int)(ndiv) - (int)(nassign)] = 5;
 cprio.A[(int)(nmod) - (int)(nassign)] = 5;
 cprio.A[(int)(nquot) - (int)(nassign)] = 5;
 cprio.A[(int)(nnot) - (int)(nassign)] = 6;
 cprio.A[(int)(numinus) - (int)(nassign)] = 6;
 cprio.A[(int)(nuplus) - (int)(nassign)] = 7;
 cprio.A[(int)(nindex) - (int)(nassign)] = 7;
 cprio.A[(int)(nselect) - (int)(nassign)] = 7;
 cprio.A[(int)(nderef) - (int)(nassign)] = 7;
 cprio.A[(int)(ncall) - (int)(nassign)] = 7;
 cprio.A[(int)(nid) - (int)(nassign)] = 7;
 cprio.A[(int)(nchar) - (int)(nassign)] = 7;
 cprio.A[(int)(ninteger) - (int)(nassign)] = 7;
 cprio.A[(int)(nreal) - (int)(nassign)] = 7;
 cprio.A[(int)(nstring) - (int)(nassign)] = 7;
 cprio.A[(int)(nnil) - (int)(nassign)] = 7;
 pprio.A[(int)(nassign) - (int)(nassign)] = 0;
 pprio.A[(int)(nformat) - (int)(nassign)] = 0;
 pprio.A[(int)(nrange) - (int)(nassign)] = 1;
 pprio.A[(int)(nin) - (int)(nassign)] = 1;
 pprio.A[(int)(neq) - (int)(nassign)] = 1;
 pprio.A[(int)(nne) - (int)(nassign)] = 1;
 pprio.A[(int)(nlt) - (int)(nassign)] = 1;
 pprio.A[(int)(nle) - (int)(nassign)] = 1;
 pprio.A[(int)(ngt) - (int)(nassign)] = 1;
 pprio.A[(int)(nge) - (int)(nassign)] = 1;
 pprio.A[(int)(nor) - (int)(nassign)] = 2;
 pprio.A[(int)(nplus) - (int)(nassign)] = 2;
 pprio.A[(int)(nminus) - (int)(nassign)] = 2;
 pprio.A[(int)(nand) - (int)(nassign)] = 3;
 pprio.A[(int)(nmul) - (int)(nassign)] = 3;
 pprio.A[(int)(ndiv) - (int)(nassign)] = 3;
 pprio.A[(int)(nmod) - (int)(nassign)] = 3;
 pprio.A[(int)(nquot) - (int)(nassign)] = 3;
 pprio.A[(int)(nnot) - (int)(nassign)] = 4;
 pprio.A[(int)(numinus) - (int)(nassign)] = 4;
 pprio.A[(int)(nuplus) - (int)(nassign)] = 5;
 pprio.A[(int)(nset) - (int)(nassign)] = 6;
 pprio.A[(int)(nindex) - (int)(nassign)] = 6;
 pprio.A[(int)(nselect) - (int)(nassign)] = 6;
 pprio.A[(int)(nderef) - (int)(nassign)] = 6;
 pprio.A[(int)(ncall) - (int)(nassign)] = 6;
 pprio.A[(int)(nid) - (int)(nassign)] = 6;
 pprio.A[(int)(nchar) - (int)(nassign)] = 6;
 pprio.A[(int)(ninteger) - (int)(nassign)] = 6;
 pprio.A[(int)(nreal) - (int)(nassign)] = 6;
 pprio.A[(int)(nstring) - (int)(nassign)] = 6;
 pprio.A[(int)(nnil) - (int)(nassign)] = 6;
 defname(cabort, *((keyword *)STRALIGN("abort     ")));
 defname(cbreak, *((keyword *)STRALIGN("break     ")));
 defname(ccontinue, *((keyword *)STRALIGN("continue  ")));
 defname(cdefine, *((keyword *)STRALIGN("define    ")));
 defname(cdefault, *((keyword *)STRALIGN("default   ")));
 defname(cdouble, *((keyword *)STRALIGN("double    ")));
 defname(cedata, *((keyword *)STRALIGN("edata     ")));
 defname(cenum, *((keyword *)STRALIGN("enum      ")));
 defname(cetext, *((keyword *)STRALIGN("etext     ")));
 defname(cextern, *((keyword *)STRALIGN("extern    ")));
 defname(cfclose, *((keyword *)STRALIGN("fclose    ")));
 defname(cfflush, *((keyword *)STRALIGN("fflush    ")));
 defname(cfgetc, *((keyword *)STRALIGN("fgetc     ")));
 defname(cfloat, *((keyword *)STRALIGN("float     ")));
 defname(cfloor, *((keyword *)STRALIGN("floor     ")));
 defname(cfprintf, *((keyword *)STRALIGN("fprintf   ")));
 defname(cfputc, *((keyword *)STRALIGN("fputc     ")));
 defname(cfread, *((keyword *)STRALIGN("fread     ")));
 defname(cfscanf, *((keyword *)STRALIGN("fscanf    ")));
 defname(cfwrite, *((keyword *)STRALIGN("fwrite    ")));
 defname(cgetc, *((keyword *)STRALIGN("getc      ")));
 defname(cgetpid, *((keyword *)STRALIGN("getpid    ")));
 defname(cint, *((keyword *)STRALIGN("int       ")));
 defname(cinclude, *((keyword *)STRALIGN("include   ")));
 defname(clong, *((keyword *)STRALIGN("long      ")));
 defname(clog, *((keyword *)STRALIGN("log       ")));
 defname(cmain, *((keyword *)STRALIGN("main      ")));
 defname(cmalloc, *((keyword *)STRALIGN("malloc    ")));
 defname(cprintf, *((keyword *)STRALIGN("printf    ")));
 defname(cpower, *((keyword *)STRALIGN("pow       ")));
 defname(cputc, *((keyword *)STRALIGN("putc      ")));
 defname(cread, *((keyword *)STRALIGN("read      ")));
 defname(creturn, *((keyword *)STRALIGN("return    ")));
 defname(cregister, *((keyword *)STRALIGN("register  ")));
 defname(crewind, *((keyword *)STRALIGN("rewind    ")));
 defname(cscanf, *((keyword *)STRALIGN("scanf     ")));
 defname(csetbits, *((keyword *)STRALIGN("setbits   ")));
 defname(csetword, *((keyword *)STRALIGN("setword   ")));
 defname(csetptr, *((keyword *)STRALIGN("setptr    ")));
 defname(cshort, *((keyword *)STRALIGN("short     ")));
 defname(csigned, *((keyword *)STRALIGN("signed    ")));
 defname(csizeof, *((keyword *)STRALIGN("sizeof    ")));
 defname(csprintf, *((keyword *)STRALIGN("sprintf   ")));
 defname(cstatic, *((keyword *)STRALIGN("static    ")));
 defname(cstdin, *((keyword *)STRALIGN("stdin     ")));
 defname(cstdout, *((keyword *)STRALIGN("stdout    ")));
 defname(cstderr, *((keyword *)STRALIGN("stderr    ")));
 defname(cstrncmp, *((keyword *)STRALIGN("strncmp   ")));
 defname(cstrncpy, *((keyword *)STRALIGN("strncpy   ")));
 defname(cstruct, *((keyword *)STRALIGN("struct    ")));
 defname(cswitch, *((keyword *)STRALIGN("switch    ")));
 defname(ctypedef, *((keyword *)STRALIGN("typedef   ")));
 defname(cundef, *((keyword *)STRALIGN("undef     ")));
 defname(cungetc, *((keyword *)STRALIGN("ungetc    ")));
 defname(cunion, *((keyword *)STRALIGN("union     ")));
 defname(cunlink, *((keyword *)STRALIGN("unlink    ")));
 defname(cfseek, *((keyword *)STRALIGN("fseek     ")));
 defname(cgetchar, *((keyword *)STRALIGN("getchar   ")));
 defname(cputchar, *((keyword *)STRALIGN("putchar   ")));
 defname(cunsigned, *((keyword *)STRALIGN("unsigned  ")));
 defname(cwrite, *((keyword *)STRALIGN("write     ")));
 defid(nfunc, dabs, *((keyword *)STRALIGN("abs       ")));
 defid(nfunc, darctan, *((keyword *)STRALIGN("arctan    ")));
 defid(nvar, dargc, *((keyword *)STRALIGN("argc      ")));
 defid(nproc, dargv, *((keyword *)STRALIGN("argv      ")));
 defid(nscalar, dboolean, *((keyword *)STRALIGN("boolean   ")));
 defid(ntype, dchar, *((keyword *)STRALIGN("char      ")));
 defid(nfunc, dchr, *((keyword *)STRALIGN("chr       ")));
 defid(nproc, dclose, *((keyword *)STRALIGN("close     ")));
 defid(nfunc, dcos, *((keyword *)STRALIGN("cos       ")));
 defid(nproc, ddispose, *((keyword *)STRALIGN("dispose   ")));
 defid(nid, dfalse, *((keyword *)STRALIGN("false     ")));
 defid(nfunc, deof, *((keyword *)STRALIGN("eof       ")));
 defid(nfunc, deoln, *((keyword *)STRALIGN("eoln      ")));
 defid(nproc, dexit, *((keyword *)STRALIGN("exit      ")));
 defid(nfunc, dexp, *((keyword *)STRALIGN("exp       ")));
 defid(nproc, dflush, *((keyword *)STRALIGN("flush     ")));
 defid(nproc, dget, *((keyword *)STRALIGN("get       ")));
 defid(nproc, dhalt, *((keyword *)STRALIGN("halt      ")));
 defid(nvar, dinput, *((keyword *)STRALIGN("input     ")));
 defid(ntype, dinteger, *((keyword *)STRALIGN("integer   ")));
 defid(nfunc, dln, *((keyword *)STRALIGN("ln        ")));
 defid(nconst, dmaxint, *((keyword *)STRALIGN("maxint    ")));
 defid(nproc, dmessage, *((keyword *)STRALIGN("message   ")));
 defid(nproc, dnew, *((keyword *)STRALIGN("new       ")));
 defid(nfunc, dodd, *((keyword *)STRALIGN("odd       ")));
 defid(nfunc, dord, *((keyword *)STRALIGN("ord       ")));
 defid(nvar, doutput, *((keyword *)STRALIGN("output    ")));
 defid(nproc, dpack, *((keyword *)STRALIGN("pack      ")));
 defid(nproc, dpage, *((keyword *)STRALIGN("page      ")));
 defid(nfunc, dpred, *((keyword *)STRALIGN("pred      ")));
 defid(nproc, dput, *((keyword *)STRALIGN("put       ")));
 defid(nproc, dread, *((keyword *)STRALIGN("read      ")));
 defid(nproc, dreadln, *((keyword *)STRALIGN("readln    ")));
 defid(ntype, dreal, *((keyword *)STRALIGN("real      ")));
 defid(nproc, dreset, *((keyword *)STRALIGN("reset     ")));
 defid(nproc, drewrite, *((keyword *)STRALIGN("rewrite   ")));
 defid(nfunc, dround, *((keyword *)STRALIGN("round     ")));
 defid(nfunc, dsin, *((keyword *)STRALIGN("sin       ")));
 defid(nfunc, dsqr, *((keyword *)STRALIGN("sqr       ")));
 defid(nfunc, dsqrt, *((keyword *)STRALIGN("sqrt      ")));
 defid(nfunc, dsucc, *((keyword *)STRALIGN("succ      ")));
 defid(ntype, dtext, *((keyword *)STRALIGN("text      ")));
 defid(nid, dtrue, *((keyword *)STRALIGN("true      ")));
 defid(nfunc, dtrunc, *((keyword *)STRALIGN("trunc     ")));
 defid(nfunc, dtan, *((keyword *)STRALIGN("tan       ")));
 defid(nproc, dunpack, *((keyword *)STRALIGN("unpack    ")));
 defid(nproc, dwrite, *((keyword *)STRALIGN("write     ")));
 defid(nproc, dwriteln, *((keyword *)STRALIGN("writeln   ")));
 defid(nfield, dzinit, *((keyword *)STRALIGN("$nit      ")));
 defid(ntype, dztring, *((keyword *)STRALIGN("$ztring   ")));
 deftab.A[(int)(dboolean)]->U.V14.tbind->U.V17.tscalid = deftab.A[(int)(dfalse)];
 deftab.A[(int)(dfalse)]->tnext = deftab.A[(int)(dtrue)];
 currsym.st = sinteger;
 currsym.U.V3.vint = maxint;
 deftab.A[(int)(dmaxint)]->U.V14.tbind = mklit();
 deftab.A[(int)(dargc)]->U.V14.tbind = deftab.A[(int)(dinteger)]->U.V14.tbind;
 deftab.A[(int)(dinput)]->U.V14.tbind = deftab.A[(int)(dtext)]->U.V14.tbind;
 deftab.A[(int)(doutput)]->U.V14.tbind = deftab.A[(int)(dtext)]->U.V14.tbind;
 {
  pretyps B83 = tnone,
   B84 = terror;

  if ((int)(B83) <= (int)(B84))
   for (t = B83; ; t = (pretyps)((int)(t)+1)) {
    switch ((int)(t)) {
      case tboolean:
     typnods.A[(int)(t)] = deftab.A[(int)(dboolean)]->U.V14.tbind;
     break ;
      case tchar:
     typnods.A[(int)(t)] = deftab.A[(int)(dchar)]->U.V14.tbind;
     break ;
      case tinteger:
     typnods.A[(int)(t)] = deftab.A[(int)(dinteger)]->U.V14.tbind;
     break ;
      case treal:
     typnods.A[(int)(t)] = deftab.A[(int)(dreal)]->U.V14.tbind;
     break ;
      case ttext:
     typnods.A[(int)(t)] = deftab.A[(int)(dtext)]->U.V14.tbind;
     break ;
      case tstring:
     typnods.A[(int)(t)] = deftab.A[(int)(dztring)]->U.V14.tbind;
     break ;
      case tnil:  case tset:  case tpoly:  case tnone:
     typnods.A[(int)(t)] = mknode(npredef);
     break ;
      case terror:
     break ;
      default:
     Caseerror(Line);
    }
    if (Member((unsigned)(t), Conset[184]))
     typnods.A[(int)(t)]->U.V12.tobtyp = t;
    if (t == B84) break;
   }
 }
 fixinit(defnams.A[(int)(dzinit)]->U.V6.lid->istr);
 deftab.A[(int)(dzinit)]->U.V14.tbind = deftab.A[(int)(dinteger)]->U.V14.tbind;
 {
  predefs B85 = dabs,
   B86 = dztring;

  if ((int)(B85) <= (int)(B86))
   for (d = B85; ; d = (predefs)((int)(d)+1)) {
    linkup((treeptr)NIL, deftab.A[(int)(d)]);
    if (d == B86) break;
   }
 }
 deftab.A[(int)(dchr)]->U.V13.tfuntyp = typnods.A[(int)(tchar)];
 deftab.A[(int)(deof)]->U.V13.tfuntyp = typnods.A[(int)(tboolean)];
 deftab.A[(int)(deoln)]->U.V13.tfuntyp = typnods.A[(int)(tboolean)];
 deftab.A[(int)(dodd)]->U.V13.tfuntyp = typnods.A[(int)(tboolean)];
 deftab.A[(int)(dord)]->U.V13.tfuntyp = typnods.A[(int)(tinteger)];
 deftab.A[(int)(dround)]->U.V13.tfuntyp = typnods.A[(int)(tinteger)];
 deftab.A[(int)(dtrunc)]->U.V13.tfuntyp = typnods.A[(int)(tinteger)];
 deftab.A[(int)(darctan)]->U.V13.tfuntyp = typnods.A[(int)(treal)];
 deftab.A[(int)(dcos)]->U.V13.tfuntyp = typnods.A[(int)(treal)];
 deftab.A[(int)(dsin)]->U.V13.tfuntyp = typnods.A[(int)(treal)];
 deftab.A[(int)(dtan)]->U.V13.tfuntyp = typnods.A[(int)(treal)];
 deftab.A[(int)(dsqrt)]->U.V13.tfuntyp = typnods.A[(int)(treal)];
 deftab.A[(int)(dexp)]->U.V13.tfuntyp = typnods.A[(int)(treal)];
 deftab.A[(int)(dln)]->U.V13.tfuntyp = typnods.A[(int)(treal)];
 deftab.A[(int)(dsqr)]->U.V13.tfuntyp = typnods.A[(int)(tpoly)];
 deftab.A[(int)(dabs)]->U.V13.tfuntyp = typnods.A[(int)(tpoly)];
 deftab.A[(int)(dpred)]->U.V13.tfuntyp = typnods.A[(int)(tpoly)];
 deftab.A[(int)(dsucc)]->U.V13.tfuntyp = typnods.A[(int)(tpoly)];
 deftab.A[(int)(dargv)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(ddispose)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(dexit)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(dget)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(dhalt)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(dnew)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(dpack)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(dput)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(dread)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(dreadln)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(dreset)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(drewrite)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(dwrite)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(dwriteln)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(dmessage)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 deftab.A[(int)(dunpack)]->U.V13.tfuntyp = typnods.A[(int)(tnone)];
 nmachdefs = 0;
 defmach(0, 255, *((machdefstr *)STRALIGN("unsigned char   ")));
 defmach(-128, 127, *((machdefstr *)STRALIGN("char            ")));
 defmach(0, 65535, *((machdefstr *)STRALIGN("unsigned short  ")));
 defmach(-32768, 32767, *((machdefstr *)STRALIGN("short           ")));
 defmach(-2147483647, 2147483647, *((machdefstr *)STRALIGN("long            ")));
}

extern void exit();

 void
error(m)
 errors m;
{
 prtmsg(m);
 exit(1);
 longjmp(J[0].jb, 9999);
}

 void
fatal(m)
 errors m;
{
 prtmsg(m);
 abort();
}

/*
**     Start of program code
*/
main()
{
 input.fp = stdin;
 output.fp = stdout;
# ifdef STDINIT
 (void)(Getx(input));
# endif
 if (setjmp(J[0].jb))
 goto L9999;
 initialize();
 if (echo)
  (void)fprintf(output.fp, "# ifdef PASCAL\n"), Putl(output, 1);
 parse();
 semcheck();
 if (echo)
  (void)fprintf(output.fp, "# else\n"), Putl(output, 1);
 lineno = 0;
 lastline = 0;
 transform();
 emit();
 if (echo)
  (void)fprintf(output.fp, "# endif\n"), Putl(output, 1);
L9999:
 ;
 exit(0);
 /* NOTREACHED */
}
# undef keytablen
# undef keywordlen
# undef C37_setbits
# undef filefill
# undef maxsetrange
# undef scalbase
# undef maxprio
# undef maxmachdefs
# undef machdeflen
# undef maxstrblk
# undef maxblkcnt
# undef maxstrstor
# undef maxtoknlen
# undef hashmax
# undef null
# undef minchar
# undef maxchar
# undef space
# undef tab1
# undef bslash
# undef quote
# undef cite
# undef xpnent
# undef percent
# undef uscore
# undef badchr
# undef okchr
# undef tabwidth
# undef echo
# undef diffcomm
# undef lazyfor
# undef unionnew
# undef realtyp
# undef align
# undef intlen
# undef fixlen
# undef indstep
/*
**     End of program code
*/
static setword Q0[] = {
 1,
 0x03FD
};
static setword Q1[] = {
 1,
 0x004C
};
static setword Q2[] = {
 1,
 0x0000
};
static setword Q3[] = {
 2,
 0x000E, 0x5210
};
static setword Q4[] = {
 2,
 0x000E, 0x1210
};
static setword Q5[] = {
 1,
 0x0C00
};
static setword Q6[] = {
 1,
 0x000C
};
static setword Q7[] = {
 2,
 0x000E, 0x0210
};
static setword Q8[] = {
 3,
 0x0000, 0x0000, 0x0060
};
static setword Q9[] = {
 4,
 0x0002, 0x0000, 0x0064, 0x0800
};
static setword Q10[] = {
 1,
 0x0C00
};
static setword Q11[] = {
 1,
 0x000C
};
static setword Q12[] = {
 4,
 0x0000, 0x0000, 0x4FF0, 0x0008
};
static setword Q13[] = {
 3,
 0x0000, 0x0000, 0x0780
};
static setword Q14[] = {
 3,
 0x0000, 0x0000, 0x0780
};
static setword Q15[] = {
 5,
 0x0000, 0x0001, 0x0000, 0x0000, 0x0001
};
static setword Q16[] = {
 3,
 0x0000, 0x0000, 0x07E0
};
static setword Q17[] = {
 3,
 0x0000, 0x0000, 0xB000
};
static setword Q18[] = {
 4,
 0x0000, 0x0008, 0x0000, 0x0040
};
static setword Q19[] = {
 3,
 0x0000, 0x0000, 0xB560
};
static setword Q20[] = {
 4,
 0x0000, 0x0000, 0x4FF0, 0x0008
};
static setword Q21[] = {
 1,
 0x0C00
};
static setword Q22[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1700
};
static setword Q23[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1700
};
static setword Q24[] = {
 3,
 0x0000, 0x0040, 0x4000
};
static setword Q25[] = {
 1,
 0x000E
};
static setword Q26[] = {
 1,
 0x000E
};
static setword Q27[] = {
 1,
 0x000C
};
static setword Q28[] = {
 1,
 0x000E
};
static setword Q29[] = {
 1,
 0x000E
};
static setword Q30[] = {
 2,
 0x8000, 0x0045
};
static setword Q31[] = {
 1,
 0x000E
};
static setword Q32[] = {
 1,
 0x000E
};
static setword Q33[] = {
 2,
 0x8000, 0x000F
};
static setword Q34[] = {
 3,
 0x0000, 0x0000, 0x0560
};
static setword Q35[] = {
 3,
 0x0000, 0x0000, 0x0060
};
static setword Q36[] = {
 2,
 0x43A0, 0x0040
};
static setword Q37[] = {
 1,
 0x0300
};
static setword Q38[] = {
 1,
 0x000C
};
static setword Q39[] = {
 1,
 0x000C
};
static setword Q40[] = {
 1,
 0x0804
};
static setword Q41[] = {
 1,
 0x0408
};
static setword Q42[] = {
 1,
 0x0C0C
};
static setword Q43[] = {
 2,
 0x0000, 0x0003
};
static setword Q44[] = {
 2,
 0x0000, 0x0003
};
static setword Q45[] = {
 2,
 0x0000, 0x0003
};
static setword Q46[] = {
 2,
 0x0000, 0x0003
};
static setword Q47[] = {
 1,
 0x0300
};
static setword Q48[] = {
 1,
 0x000C
};
static setword Q49[] = {
 1,
 0x0804
};
static setword Q50[] = {
 1,
 0x0408
};
static setword Q51[] = {
 1,
 0x0300
};
static setword Q52[] = {
 1,
 0x0300
};
static setword Q53[] = {
 1,
 0x0300
};
static setword Q54[] = {
 1,
 0x4380
};
static setword Q55[] = {
 2,
 0x0020, 0x0040
};
static setword Q56[] = {
 3,
 0x4010, 0x0C00, 0x000A
};
static setword Q57[] = {
 5,
 0x0300, 0x0000, 0x0004, 0x2000, 0x0001
};
static setword Q58[] = {
 4,
 0x0300, 0x0000, 0x0004, 0x2000
};
static setword Q59[] = {
 3,
 0x0000, 0x0000, 0x0080
};
static setword Q60[] = {
 4,
 0x0040, 0x0000, 0x0000, 0x0800
};
static setword Q61[] = {
 1,
 0x0040
};
static setword Q62[] = {
 3,
 0x0080, 0x0000, 0x0001
};
static setword Q63[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x4000
};
static setword Q64[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q65[] = {
 4,
 0x0000, 0x0000, 0x0004, 0x2000
};
static setword Q66[] = {
 1,
 0x0040
};
static setword Q67[] = {
 2,
 0x0000, 0x8000
};
static setword Q68[] = {
 4,
 0x0300, 0x0000, 0x0004, 0x2000
};
static setword Q69[] = {
 4,
 0x930C, 0x2001, 0x0834, 0x2000
};
static setword Q70[] = {
 4,
 0x930C, 0x2001, 0x0834, 0x3000
};
static setword Q71[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1800
};
static setword Q72[] = {
 2,
 0x0200, 0x0100
};
static setword Q73[] = {
 3,
 0x0200, 0x0100, 0x3C80
};
static setword Q74[] = {
 2,
 0x0000, 0x0040
};
static setword Q75[] = {
 4,
 0x0000, 0x0000, 0x4FF0, 0x0008
};
static setword Q76[] = {
 4,
 0x03E1, 0x80CA, 0xF005, 0xBFFE
};
static setword Q77[] = {
 4,
 0x0000, 0x0030, 0x3F80, 0x001A
};
static setword Q78[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x0478
};
static setword Q79[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x0478
};
static setword Q80[] = {
 1,
 0x0C0C
};
static setword Q81[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q82[] = {
 5,
 0x0000, 0x0000, 0x0000, 0x000B, 0x0001
};
static setword Q83[] = {
 5,
 0x03E1, 0x80CA, 0xF005, 0xFFFF, 0x0001
};
static setword Q84[] = {
 4,
 0x930C, 0x2001, 0x0834, 0x2000
};
static setword Q85[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1000
};
static setword Q86[] = {
 4,
 0x930C, 0x2001, 0x08B4, 0x2000
};
static setword Q87[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x2004
};
static setword Q88[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q89[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1000
};
static setword Q90[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1002
};
static setword Q91[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x2004
};
static setword Q92[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x2006
};
static setword Q93[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x2004
};
static setword Q94[] = {
 3,
 0x0002, 0x0000, 0x0800
};
static setword Q95[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q96[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1000
};
static setword Q97[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q98[] = {
 3,
 0x4000, 0x0400, 0x0808
};
static setword Q99[] = {
 3,
 0x0002, 0x0000, 0x0800
};
static setword Q100[] = {
 2,
 0x0000, 0x0040
};
static setword Q101[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x2010
};
static setword Q102[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q103[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x2010
};
static setword Q104[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q105[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x0008
};
static setword Q106[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1000
};
static setword Q107[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q108[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x8000
};
static setword Q109[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q110[] = {
 3,
 0x4004, 0x0400, 0x0040
};
static setword Q111[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x2000
};
static setword Q112[] = {
 1,
 0x2400
};
static setword Q113[] = {
 3,
 0x4014, 0x0404, 0x000A
};
static setword Q114[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x2000
};
static setword Q115[] = {
 3,
 0x6414, 0x0404, 0x000A
};
static setword Q116[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x2000
};
static setword Q117[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q118[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q119[] = {
 3,
 0x4004, 0x0400, 0x0840
};
static setword Q120[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1800
};
static setword Q121[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q122[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1800
};
static setword Q123[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q124[] = {
 3,
 0x4004, 0x0400, 0x0848
};
static setword Q125[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x0020
};
static setword Q126[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q127[] = {
 2,
 0x0000, 0x0040
};
static setword Q128[] = {
 2,
 0x0000, 0x0040
};
static setword Q129[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x0010
};
static setword Q130[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x0008
};
static setword Q131[] = {
 4,
 0x0200, 0x0000, 0x0000, 0x2004
};
static setword Q132[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q133[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x0004
};
static setword Q134[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q135[] = {
 3,
 0x0000, 0x0000, 0x3C80
};
static setword Q136[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x8000
};
static setword Q137[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x8000
};
static setword Q138[] = {
 4,
 0x0200, 0x0000, 0x0000, 0x2814
};
static setword Q139[] = {
 2,
 0x0802, 0x5000
};
static setword Q140[] = {
 4,
 0x0802, 0x5200, 0x3C80, 0x0003
};
static setword Q141[] = {
 4,
 0x0200, 0x0000, 0x0000, 0x2004
};
static setword Q142[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x0002
};
static setword Q143[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1800
};
static setword Q144[] = {
 2,
 0x0000, 0x0040
};
static setword Q145[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q146[] = {
 4,
 0x0000, 0x0040, 0x0000, 0x1000
};
static setword Q147[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q148[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1000
};
static setword Q149[] = {
 3,
 0x0000, 0x0000, 0x0880
};
static setword Q150[] = {
 3,
 0x0000, 0x0000, 0x0980
};
static setword Q151[] = {
 3,
 0x0000, 0x0000, 0x3000
};
static setword Q152[] = {
 3,
 0x4004, 0x0400, 0x084A
};
static setword Q153[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x2000
};
static setword Q154[] = {
 3,
 0x0000, 0x0000, 0x3F80
};
static setword Q155[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x0020
};
static setword Q156[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q157[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1824
};
static setword Q158[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q159[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x1824
};
static setword Q160[] = {
 3,
 0x4014, 0x0400, 0x000A
};
static setword Q161[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x2800
};
static setword Q162[] = {
 3,
 0x0000, 0x0000, 0x0080
};
static setword Q163[] = {
 3,
 0x0000, 0x0000, 0x0040
};
static setword Q164[] = {
 4,
 0x0000, 0x0000, 0x0040, 0x2000
};
static setword Q165[] = {
 3,
 0x0000, 0x0000, 0x0040
};
static setword Q166[] = {
 5,
 0x0000, 0x0000, 0x0000, 0x0000, 0x0001
};
static setword Q167[] = {
 3,
 0x4014, 0x0404, 0x000A
};
static setword Q168[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x2000
};
static setword Q169[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q170[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x2002
};
static setword Q171[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q172[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x0804
};
static setword Q173[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q174[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x0804
};
static setword Q175[] = {
 3,
 0x0000, 0x0000, 0x0800
};
static setword Q176[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x0804
};
static setword Q177[] = {
 3,
 0x0004, 0x0000, 0x0040
};
static setword Q178[] = {
 1,
 0x0018
};
static setword Q179[] = {
 1,
 0x00C0
};
static setword Q180[] = {
 1,
 0x001A
};
static setword Q181[] = {
 1,
 0x001E
};
static setword Q182[] = {
 4,
 0x0000, 0x0000, 0x0000, 0x0478
};
static setword Q183[] = {
 1,
 0x001F
};
static setword Q184[] = {
 1,
 0x1FE7
};
static setword *Conset[] = {
 Q184, Q183, Q182, Q181, Q180, Q179,
 Q178, Q177, Q176, Q175, Q174, Q173,
 Q172, Q171, Q170, Q169, Q168, Q167,
 Q166, Q165, Q164, Q163, Q162, Q161,
 Q160, Q159, Q158, Q157, Q156, Q155,
 Q154, Q153, Q152, Q151, Q150, Q149,
 Q148, Q147, Q146, Q145, Q144, Q143,
 Q142, Q141, Q140, Q139, Q138, Q137,
 Q136, Q135, Q134, Q133, Q132, Q131,
 Q130, Q129, Q128, Q127, Q126, Q125,
 Q124, Q123, Q122, Q121, Q120, Q119,
 Q118, Q117, Q116, Q115, Q114, Q113,
 Q112, Q111, Q110, Q109, Q108, Q107,
 Q106, Q105, Q104, Q103, Q102, Q101,
 Q100, Q99, Q98, Q97, Q96, Q95,
 Q94, Q93, Q92, Q91, Q90, Q89,
 Q88, Q87, Q86, Q85, Q84, Q83,
 Q82, Q81, Q80, Q79, Q78, Q77,
 Q76, Q75, Q74, Q73, Q72, Q71,
 Q70, Q69, Q68, Q67, Q66, Q65,
 Q64, Q63, Q62, Q61, Q60, Q59,
 Q58, Q57, Q56, Q55, Q54, Q53,
 Q52, Q51, Q50, Q49, Q48, Q47,
 Q46, Q45, Q44, Q43, Q42, Q41,
 Q40, Q39, Q38, Q37, Q36, Q35,
 Q34, Q33, Q32, Q31, Q30, Q29,
 Q28, Q27, Q26, Q25, Q24, Q23,
 Q22, Q21, Q20, Q19, Q18, Q17,
 Q16, Q15, Q14, Q13, Q12, Q11,
 Q10, Q9, Q8, Q7, Q6, Q5,
 Q4, Q3, Q2, Q1, Q0
};

static setptr
Union(p1, p2)
 register setptr p1, p2;
{
 register int             i, j, k;
 register setptr             sp = Newset(),
                             p3 = sp;

 j = *p1;
 *p3 = j;
 if (j > *p2)
             j = *p2;
 else
             *p3 = *p2;
 k = *p1 - *p2;
 p1++, p2++, p3++;
 for (i = 0; i < j; i++)
             *p3++ = (*p1++ | *p2++);
 while (k > 0) {
             *p3++ = *p1++;
             k--;
 }
 while (k < 0) {
             *p3++ = *p2++;
             k++;
 }
 return (Saveset(sp));
}

static boolean
Member(m, sp)
 register unsigned int m;
 register setptr sp;
{
 register unsigned int i = m / (setbits+1) + 1;

 if ((i <= *sp) && (sp[i] & (1 << (m % (setbits+1)))))
             return (true);
 return (false);
}

static setptr
Insmem(m, sp)
 register unsigned int m;
 register setptr sp;
{
 register int i,
                      j = m / (setbits+1) + 1;

 if (*sp < j)
             for (i = *sp + 1, *sp = j; i <= *sp; i++)
                     sp[i] = 0;
 sp[j] |= (1 << (m % (setbits+1)));
 return (sp);
}

# ifndef SETSPACE
# define SETSPACE 256
# endif
static setptr
Currset(n,sp)
 int n;
 setptr sp;
{
 static setword Space[SETSPACE];
 static setptr Top = Space;

 switch (n) {
   case 0:
             Top = Space;
             return (0);
   case 1:
             if (&Space[SETSPACE] - Top <= 15) {
                     (void)fprintf(stderr, "Set-space exhausted\n");
                     exit(1);
             }
             *Top = 0;
             return (Top);
   case 2:
             if (Top <= &sp[*sp])
                     Top = &sp[*sp + 1];
             return (sp);
 }
 /* NOTREACHED */
}

static void
Setncpy(S1, S2, N)
 register setptr S1, S2;
 register unsigned int N;
{
 register unsigned int m;

 N /= sizeof(setword);
 *S1++ = --N;
 m = *S2++;
 while (m != 0 && N != 0) {
             *S1++ = *S2++;
             --N;
             --m;
 }
 while (N-- != 0)
             *S1++ = 0;
}

static struct Set *
Alignset(Sp)
 register unsigned short *Sp;
{
 static struct Set tmp;
 register unsigned short *tp = tmp.S;
 register int             i = *Sp;

 while (i-- >= 0)
             *tp++ = *Sp++;
 return (&tmp);
}

static struct String *
Alignstr(Cp)
 register char *Cp;
{
 static struct String tmp;
 register char *sp = tmp.A;

 while (*sp++ = *Cp++)
             ;
 return (&tmp);
}

static void
Caseerror(n)
 int n;
{
 (void)fprintf(stderr, "Missing case limb: line %d\n", n);
 exit(1);
 /* NOTREACHED */
}
