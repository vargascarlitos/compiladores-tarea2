#include "anlex.h"


char cad[5*TAMLEX];		
token t;				

FILE *archivo;	
char id[TAMLEX];		
int numLinea=1;		
char c=0;






void error( string mensaje)
{
	printf("\nLin %d: Error Lexico. %s.\n",numLinea,mensaje);
	
		while(fgetc(archivo) != '\n'){
            fgetc(archivo);    	 
    }
    numLinea++;    	
}

void sigLex()
{
	int i=0;
	int estado=0;
    int acepto=0;
	char msg[5*TAMLEX];
    
	while((c=fgetc(archivo))!=EOF)
	{
		if (c==' ')
            continue;
               
        else if(c=='\t')        
            continue;

		else if(c=='\n')
		{

			numLinea++;

		}

		else if (isalpha(c))
		{

			i=0;
			do{
				id[i]=c;
				i++;
				c=fgetc(archivo);       
			}while(isalpha(c));
			id[i]='\0';
			if (c != EOF){
				ungetc(c,archivo);}
			else
			    c=0;
			palabra_reservada(id);
            t.lexema = id;
			break;
		}

        else if (c=='"'){

            i=0;
            estado=0;
            acepto=0;
            id[i]=c;
            while(!acepto){
             	switch(estado){
             		case 0:

             			c=fgetc(archivo);
             			if(i>=TAMLEX){
             				estado = -1;
                    	}
                    	
             			else if(c=='\t' || c=='\n' || c==EOF){
             				estado = -1;
             			}
             			else if(c=='"'){
             				id[++i] = c;
             				estado = 1;
             			}
             			else{
             				id[++i]= c;
             				estado=0;
             			}
             			break;
             		case 1:						
						c=0;
						id[++i]='\0';
						acepto=1;
						t.compLex=LITERAL_CADENA;
		                t.lexema = id;
		                t.componente = nombres_comp[LITERAL_CADENA - 256]; 
						break;
					case -1:						
						if (c==EOF)
							error("No se esperaba el fin de archivo");
						else{
							ungetc(c,archivo);
							if(i>= TAMLEX) error("Longitud de Identificador excede tama�o de buffer");
							else if(c=='\n')error("Salto de linea no esperado");
							else if(c=='\t')error("Tabulacion no esperada");
							t.componente = 0;
		                    acepto=1;
		                    break;
		                }					
             	} 
            }
            break;
        }

		else if (isdigit(c))
		{
                
			i=0;
			estado=0;
			acepto=0;
			id[i]=c;
			while(!acepto)
			{
				switch(estado){
					case 0: 
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=0;
						}
						else if(c=='.'){
							id[++i]=c;
							estado=1;
						}
						else if(tolower(c)=='e'){
							id[++i]=c;
							estado=3;
						}
						else{
							estado=6;
						}
						break;
					
					case 1:
						c=fgetc(archivo);						
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						
						else{
							sprintf(msg,"No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					case 2:
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else if(tolower(c)=='e')
						{
							id[++i]=c;
		                    printf("%c = ",c);
							estado=3;
						}
						else
							estado=6;
						break;
					case 3:
						c=fgetc(archivo);
						if (c=='+' || c=='-')
						{
							id[++i]=c;
							estado=4;
						}
						else if(isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							sprintf(msg,"No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					case 4:
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							sprintf(msg,"No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					case 5:
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							estado=6;
						}break;
					case 6:
						if (c!=EOF)
							ungetc(c,archivo);
						else
							c=0;
							id[++i]='\0';
							acepto=1;
							t.compLex=LITERAL_NUM;
		                    t.lexema = id;
		                    t.componente = nombres_comp[LITERAL_NUM - 256]; 
						break;
					case -1:
						if (c==EOF)
							error("No se esperaba el fin de archivo");
						else{
							t.componente = 0;
		                    sprintf(msg,"%c no esperado",c);
			                error(msg);
		                    acepto=1;
		                }					
		                break;
		        }
			}
        	break;
		}

		else if (c==':')
		{
			t.compLex= DOS_PUNTOS;
            t.lexema = ":";			
            t.componente = nombres_comp[DOS_PUNTOS - 256];
			break;
		}
		else if (c==',')
		{
			t.compLex= COMA;
            t.lexema = "," ;
            t.componente = nombres_comp[COMA - 256];            
			break;
		}		
        else if (c=='[')
		{
			t.compLex=L_CORCHETE;
			t.lexema = "[";
            t.componente = nombres_comp[L_CORCHETE - 256];
			break;
		}
		else if (c==']')
		{
			t.compLex=R_CORCHETE;
			t.lexema = "]";
            t.componente = nombres_comp[R_CORCHETE - 256];
			break;
		}
		else if (c=='{')
		{
            t.compLex= L_LLAVE;
            t.lexema = "{";            
            t.componente = nombres_comp[L_LLAVE - 256];
			break;		
        }
        else if (c=='}')
        {
            t.compLex= R_LLAVE;
            t.lexema = "}";            
            t.componente = nombres_comp[R_LLAVE - 256];
			break;		
        }
		else if (c!=EOF)
		{
			sprintf(msg,"%c no esperado",c);
			error(msg);
		}
	}
	
    if (c==EOF)
	{
		t.compLex=EOF;
	}
	
}

void palabra_reservada(char id []){
   if (strcmp(id,"TRUE") == 0 || strcmp(id,"true") == 0 ) {
       t.compLex = PR_TRUE;
       t.lexema = id;
       t.componente = nombres_comp[PR_TRUE - 256]; 
   	}else if(strcmp(id,"FALSE") == 0 || strcmp(id,"false") == 0 ) {
       t.compLex = PR_FALSE;
       t.lexema = id;
       t.componente = nombres_comp[PR_FALSE - 256]; 
   	}else if(strcmp(id,"NULL") == 0 || strcmp(id,"null") == 0 ) {
       t.compLex = PR_NULL;
       t.lexema = id;
       t.componente = nombres_comp[PR_NULL - 256]; 
   	}
   	else{
   		ungetc(c,archivo);
   		sprintf(cad,"No se esperaba '%s'",id);
   		t.componente =0;
       	error(cad);
    }
}

