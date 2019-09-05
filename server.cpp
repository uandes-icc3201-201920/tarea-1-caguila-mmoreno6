#include <iostream>
#include <sstream>
#include <stdio.h>
#include <memory>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "util.h"
#include <stdlib.h>
#include <string.h>
using namespace std;

// Almacenamiento KV
KVStore db;

char* socket_path = (char *)"\0hidden";

int main(int argc, char** argv) {	
	struct sockaddr_un addr;
	char buf[100];
	int fd,cl,rc;
	int sflag = 0;
	int opt;
	int ex = 0;

	// Procesar opciones de linea de comando
    while ((opt = getopt (argc, argv, "s:")) != -1) {
        switch (opt)
		{
			/* Procesar el flag s si el usuario lo ingresa */
			case 's':
				socket_path=argv[2];
				sflag = 1;
				break;
			default:
				cout << "input invalido" << endl;
				return 0;
          }	    	
    }
	while (1) {
  if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    cout <<"socket error" <<endl;
    exit(-1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  if (*socket_path == '\0') {
	*addr.sun_path = '\0';
    socket_path = (char *)"/tmp/db.tuples.sock";
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
  } else {
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    unlink(socket_path);
  }

  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    cout <<"bind error: socket ya existe en esta direccion" << endl;
    exit(-1);
  }
  if (listen(fd, 5) == -1) {
    cout <<"listen error" << endl;
    exit(-1);
  }
	else {
		cout << "Escuchando conexion del cliente..." << endl;
	}

  while (1) {
    if ( (cl = accept(fd, NULL, NULL)) == -1) {
      cout << "accept error" <<endl;
      continue;
    }
		else {
			cout << "Conexion con cliente aceptada\nEsperando comandos del clientes..." <<endl;
		}

    while ( (rc=read(cl,buf,sizeof(buf))) > 0) { 
 			if (strncmp(buf, "quit", 4)==0 && strlen(buf) == 4) {
			cout << "quiting..." << endl;
			ex = 1;
			break;
				}
      printf("%.*s\n", rc, buf);
      string copy(buf);
      //Identifica los valores a y b de insert(a,b)
			if (strncmp(buf, "insert", 6)==0) {
				string parentesis = copy.substr(6);	
				int posComa = parentesis.find(',');
				if (posComa != string::npos)
				  {		
					int posUltimoParentesis = parentesis.find(')');
					string primerNumero = parentesis.substr(1,posComa-1);
					string segundoNumero = parentesis.substr(posComa+1, (posUltimoParentesis-posComa-1));
					cout << "key: "+primerNumero << endl;
					cout << "value: "+segundoNumero << endl;
					*buf = '\0';
				}
				else 
				{
					int ultParentesis = parentesis.find(')');
					string primerNumero = parentesis.substr(1,ultParentesis-1);
					cout << "value: "+primerNumero << endl;
					*buf = '\0';
			}
			}
			//Identifica el valor a de get(a)
			else if (strncmp(buf, "get", 3)==0) {
				string parentesis = copy.substr(3);
				int ultParentesis = parentesis.find(')');
				string primerNumero = parentesis.substr(1,ultParentesis-1);
				cout << "key: " +primerNumero << endl;
				*buf = '\0';
				}
			//Identifica el valor a de peek(a)
			else if (strncmp(buf, "peek", 4)==0) {
				string parentesis = copy.substr(4);
				int ultParentesis = parentesis.find(')');
				string primerNumero = parentesis.substr(1,ultParentesis-1);
				cout << "key: "+primerNumero << endl;
				*buf = '\0';
				}
			//Identifica el valor a y b de update(a,b)
			else if (strncmp(buf, "update", 6)==0) {
				string parentesis = copy.substr(6);	
				int posComa = parentesis.find(',');
				int posUltimoParentesis = parentesis.find(')');
				string primerNumero = parentesis.substr(1,posComa-1);
				string segundoNumero = parentesis.substr(posComa+1, (posUltimoParentesis-posComa-1));
				cout << "key: "+primerNumero << endl;
				cout << "value: "+segundoNumero << endl;
				*buf = '\0';
				}
			//Identifica el valor a de delete(a)
			else if (strncmp(buf, "delete", 6)==0) {
				string parentesis = copy.substr(6);
				int ultParentesis = parentesis.find(')');
				string primerNumero = parentesis.substr(1,ultParentesis-1);
				cout << "key: "+primerNumero << endl;
				*buf = '\0';
				}
			//Retorna list de keys
			else if (strncmp(buf, "list", 4)==0) {
				cout << "lista: " << endl;
				*buf = '\0';
				}
			}
    if (rc == -1) {
      cout << "read error" << endl;
      exit(-1);
    }
    else if (rc == 0) {
      cout << "EOF\n" << endl;
      close(cl);
    }
    if (ex == 1) break;
  }
  }
	
	// Uso elemental del almacenamiento KV:
	
	// Creamos un arreglo de bytes a mano
	byte data[] = { 0x01, 0x01, 0x01, 0x01, 0x01 };

	// Luego un vector utilizando el arreglo de bytes
	vector<byte> vdata(data, data + sizeof(data));
	
	// Creamos el valor
	Value val = { 5, vdata };
	
	// Insertamos un par clave, valor directamente
	// en el mapa KV
	
	// Nota: Debiera diseñarse una solución más robusta con una interfaz
	// adecuada para acceder a la estructura.
	db.insert(std::pair<unsigned long, Value>(1000, val));
		
	// Imprimir lo que hemos agregado al mapa KV.
	cout << db[1000].size << " " << (int) db[1000].data[0] << endl;
	
	return 0;
}
