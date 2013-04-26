#include "strcalc.h"

//Modo radianes (rad=1) modo normal (rad=0)
char rad=0;
char *fstart=0,*errpos=0;

//Funciones válidas
FUNC func[]={
  {"asin(",'a'},{"acos(",'b'},{"atan(",'d'},{"abs(",'j'},{"sin(",'s'},
  {"sinh(",'f'},{"cos(",'c'},{"cosh(",'g'},{"tan(",'t'},{"tanh(",'h'},
  {"ln(",'n'},{"log(",'l'},{"e(",'e'},{"rand(",'r'},{"rnd(",'u'},{"int(",'i'}
};

//********************************************************
// Devuelve la posición del error... (si hubo alguno)
//********************************************************
unsigned char GetFunctionErrorPos(void){
  return (unsigned char)(errpos-fstart);
}
//********************************************************
// Devuelve el error... (si hubo alguno)
//********************************************************
char *GetFunctionError(void){
  return errpos;
}
//********************************************************
// Libera lista en caso de error...
//********************************************************
void freelist(PCALCS prim){
  PCALCS tmp;
  while(prim){
    tmp=prim;prim=prim->sig;free(tmp);
  }
}
//********************************************************
// Calcula lista de operaciones-operandos...
//********************************************************
double CalcOp(PCALCS pcy){
  double res=0;PCALCS tpcy;
  while(pcy){
    switch(pcy->op){
      case '=':res=pcy->val;break;
      case '^':res=pow(res,pcy->val);break;
      case '%':res=fmod(res,pcy->val);break;
      case '*':res=res*pcy->val;break;
      case '/':res=res/pcy->val;break;
      case '+':res=res+pcy->val;break;
      case '-':res=res-pcy->val;break;
      break;
      //Funciones trigonometricas
      case 'a':
        if(rad)res=asin(pcy->val);
        else res=(asin(pcy->val)*180)/M_PI;
      break;
      case 'b':
        if(rad)res=acos(pcy->val);
        else res=(acos(pcy->val)*180)/M_PI;
      break;
      case 'd':
        if(rad)res=atan(pcy->val);
        else res=(atan(pcy->val)*180)/M_PI;
      break;
      case 's':
        if(rad)res=sin(pcy->val);
        else res=sin((pcy->val*M_PI)/180);
      break;
      case 'c':
        if(rad)res=cos(pcy->val);
        else res=cos((pcy->val*M_PI)/180);
      break;
      case 't':
        if(rad)res=tan(pcy->val);
        else res=tan((pcy->val*M_PI)/180);
      case 'f':res=sinh(pcy->val);break;
      case 'g':res=cosh(pcy->val);break;
      case 'h':res=tanh(pcy->val);break;
      //Otras funciones
      case 'n':res=log(pcy->val);break;
      case 'l':res=log10(pcy->val);break;
      case 'e':res=exp(pcy->val);break;
      case 'j':res=fabs(pcy->val);break;
      case 'r':res=fmod(rand(),pcy->val);break;
      case 'i':modf(pcy->val,&res);break;
      case 'u':res=ceill(pcy->val);break;
      default:res=0;
    }
    tpcy=pcy;pcy=pcy->sig;free(tpcy);
  }
  return res;
}
//********************************************************
// Obtiene el número del String...
//********************************************************
double GetNum(char **txt){
  double tmp=0;unsigned char des=0;
  while((**txt)&&(isdigit(**txt)||((**txt)=='.')||((**txt)==' '))){
    //Controlamos espacios en blanco...
    if(**txt==' '){
      while(**txt==' ')(*txt)++;
      continue;
    }
    if((**txt)=='.')
      if(des){
        errpos=*txt;
        return 0;
      }else{
        des=1;
        (*txt)++;
        continue;
      }
    if(des)
      tmp=tmp+((*((*txt)++)-0x30)/pow(10,des++));
    else 
      tmp=(tmp*10)+(*((*txt)++)-0x30);
  }
  if(des&&tmp==0){
    errpos=*txt;
    return 0;
  }
  else return tmp;
}
//********************************************************
// Genera lista de Operadores-Operandos y administra
// resultados... (recursivo)
//********************************************************
static double Calc(char **py,char lop,double lval){
  char *pyt,*opy,cp,trig,tmp[256];
  unsigned char av,flen,nfunc=sizeof(func)/sizeof(func[0]);
  PCALCS prim=NULL,tp=NULL,atp=NULL,tq,tt;
  while(**py){
    //Controlamos espacios en blanco...
    while(**py==' ')*py++;
    //Nuevo elemento...
    if(tp)atp=tp;
    tp=(PCALCS)malloc(sizeof(CALCS));
    if(!prim)prim=tp;else tq->sig=tp;
    tq=tp;tp->sig=NULL;  
    //Operación?
    if(lval){//Argumento enviado...
      tp->val=lval;tp->op='=';lval=0;continue;
    }
    //primer operación...
    if(isoper(**py))//Es una operación?
      switch(**py){
        //Suma Resta prioridad 3...
        case '+':case '-':
          switch(lop){
            case '*':case '^':case '%':
              atp->sig=NULL;free(tp);
              return CalcOp(prim);
            case '+':
              tp->op=*(*py)++;
            break;
            default:
              goto OnError;//Error...
          }
        break;
        //Multiplicación División prioridad 2...
        case '*':case '/':
          switch(lop){
            case '^':case '%':
              atp->sig=NULL;free(tp);
              return CalcOp(prim);
            case '*':tp->op=*(*py)++;break;
            case '+':
              atp->val=Calc(&(*py),'*',atp->val);
              atp->sig=NULL;free(tp);tp=NULL;tq=atp;
              if(errpos!=fstart)
                goto OnError;//Error...
            continue;
            default:
              goto OnError;//Error...
          }
        break;
        //Modulo prioridad 1...
        case '%':
          switch(lop){
            case '^':
              atp->sig=NULL;free(tp);
              return CalcOp(prim);
            case '%':tp->op=*(*py)++;break;
            case '+':case '*':
              atp->val=Calc(&(*py),'%',atp->val);
              atp->sig=NULL;free(tp);tp=NULL;tq=atp;
              if(errpos!=fstart)
                goto OnError;//Error...
            continue;
            default:
              goto OnError;//Error...
          }
        break;
        //Potencia prioridad 0...
        case '^':
          switch(lop){
            case '^':tp->op=*(*py)++;break;
            case '+':case '*':case '%':
              atp->val=Calc(&(*py),'^',atp->val);
              atp->sig=NULL;free(tp);tp=NULL;tq=atp;
              if(errpos!=fstart)
                goto OnError;//Error...
            continue;
            default:
              goto OnError;//Error...
          }
        break;
        default:
          goto OnError;//Error...
      }
    else //Restantes opciones...
      tp->op='=';
    //Obtenemos argumento...
    if(isdigit(**py)||((**py)=='.')){
      tp->val=GetNum(&(*py));
      if(errpos!=fstart)
        goto OnError;//Error...    
    }else{
      trig=0;
      //Parentesis...
      if(**py=='(')goto CalcCont;
      //Funciones...
      for(av=0;av<nfunc;av++){
        flen=strlen(func[av].fname);
        if(!strncmp(*py,func[av].fname,flen)){
          (*py)+=flen-1;trig=func[av].func;
          goto CalcCont;
        }
      }
      //Tratamos Error...
      goto OnError;//Error...
CalcCont://Calculamos contenido de "()"
      pyt=tmp;cp=1;(*py)++;opy=*py;
      while(**py){
        if(**py=='(')++cp;
        if(**py==')')--cp;
        if(!cp)break;
        *(pyt++)=*(*py)++;
      }
      if(cp)
        goto OnError;//Error...
      *pyt=0;(*py)++;pyt=tmp;
      tp->val=Calc(&pyt,'+',0);
      if(errpos!=fstart){
        *py=opy;
        goto OnError;//Error...
      }
      //Resolvemos funciones...
      if(trig){
        tt=(PCALCS)malloc(sizeof(CALCS));
        tt->sig=NULL;
        tt->op=trig; 
        tt->val=tp->val;
        tp->val=CalcOp(tt);
      }
    }
  }
  return CalcOp(prim);
  OnError://Error...
    errpos=*py;
    freelist(prim);
  return 0;
}
//********************************************************
// Verifica String y llama a la función de cálculo...
//********************************************************
double Calculate(char *arg){
  fstart=errpos=arg;
  if(strlen(arg)>256){//256 caracteres Máximo...
    errpos=arg+256;
    return 0;
  }
  return Calc(&arg,'+',0);
}
