using System;
using System.Text;

namespace MainMenu
{
	/// <summary>
	/// Descripci�n breve de CheckSum.
	/// </summary>
	public class CheckSum
	{		
		public char ChkSum(string MyString)
		{
			//
			// TODO: agregar aqu� la l�gica del constructor
			//
			long s,r;
			int c,n;
			char chk;

			ASCIIEncoding AE = new ASCIIEncoding();
			byte[] ss = AE.GetBytes(MyString);
			s = 0;r = 0;n = 0;c = 2;             //inicializa variables
            n = MyString.LastIndexOf((char)10) + 1;			
			for(int k=0;k<n;k++)               //Solo la cadena de caracteres valida
			{		
				chk = (char)ss[k];
				r = ss[k]*c;					//el valor de la posici�n por el contador de posici�n
				s = s + r;                      //se suma los valores
				c++;                            //se incrementa la posici�n
				if(c==9)c = 2;                  //9 es el m�ximo contador de posici�n                
			}			
			r=(s*10) % 11;                      // mod 11  residuo			
			r += 48;                            //solo caracteres visibles
			chk = (char)r ;                     //checksum como unsigned char
			return (chk);            
		}
	}
}