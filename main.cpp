#include <Windows.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable : 4996)

#define ADDR_TIMER 0x01002FF5
#define BEG_MINES 0x01005340
#define END_MINES 0x0100567F
#define STATUS 0x01005160
#define PARA_MINAS 0x0100352D

unsigned char parartempoShellcode[] = "\x90\x90\x90\x90\x90\x90";
unsigned char voltartempoShellcode[] = "\xff\x05\x9c\x57\x00\x01";
unsigned char pararverificacaominasShellcode[] = "\xeb\x66";
unsigned char voltarverificacaominasShellcode[] = "\x74\x66";

int menu();
int parartempo();
int voltartempo();
void mostrarminas();
void imprimirminas(LPVOID buffer);
void congelarminas();
void descongelarminas();
HANDLE abrirProcesso();

int main(int argc, char *argv[]) {
	int escolha;
	setlocale(LC_ALL, "");
	escolha = menu();
	system("cls");
	if (escolha == 1) {
		parartempo();
	}
	else if (escolha == 2) {
		voltartempo();
	}
	else if (escolha == 3) {
		mostrarminas();
	}
	else if (escolha == 4) {
		congelarminas();
	}
	else if (escolha == 5) {
		descongelarminas();
	}
	else {
		printf("Escolha não reconhecida\n");
		return(0);
	}



	return(0);
}

int menu() {
	int escolha;
	printf("Escolha uma opção:\n");
	printf("1 - Parar o timer\n");
	printf("2 - Voltar o timer\n");
	printf("3 - Mostrar onde estão as minas\n");
	printf("4 - Bloquear as minas\n");
	printf("5 - Desbloquear as minas\n");

	scanf("%d", &escolha);
	return(escolha);
}

int parartempo() {
	HANDLE processo = abrirProcesso();
	DWORD ShellcodeTam = 6; //bytes do shellcode
	LPBYTE lpShellcode = (LPBYTE) malloc(ShellcodeTam);
	DWORD escrito = 0;

	if (processo == NULL) {
		printf("Não achei a janela dessa porcaria");
		exit(1);
	}
	memcpy(lpShellcode, parartempoShellcode, ShellcodeTam);

	DWORD dwWritten = 0;
	BOOL bRet = FALSE;
	bRet = WriteProcessMemory(processo, LPVOID(ADDR_TIMER), lpShellcode, ShellcodeTam, &dwWritten);
	CloseHandle(processo);

	free(lpShellcode);

	return(0);
}

int voltartempo() {
	HANDLE processo = abrirProcesso();
	DWORD ShellcodeTam = 6; //bytes do shellcode
	LPBYTE lpShellcode = (LPBYTE)malloc(ShellcodeTam);
	DWORD escrito = 0;

	if (processo == NULL) {
		printf("Não achei a janela");
		exit(1);
	}
	memcpy(lpShellcode, voltartempoShellcode, ShellcodeTam);

	DWORD dwWritten = 0;
	BOOL bRet = FALSE;
	bRet = WriteProcessMemory(processo, LPVOID(ADDR_TIMER), lpShellcode, ShellcodeTam, &dwWritten);
	CloseHandle(processo);

	free(lpShellcode);

	return(0);
}

void congelarminas() {
	HANDLE processo = abrirProcesso();
	DWORD ShellcodeTam = 2; //bytes do shellcode
	LPBYTE lpShellcode = (LPBYTE)malloc(ShellcodeTam);
	DWORD escrito = 0;

	if (processo == NULL) {
		printf("Não achei a janela dessa porcaria");
		exit(1);
	}
	memcpy(lpShellcode, pararverificacaominasShellcode, ShellcodeTam);

	DWORD dwWritten = 0;
	BOOL bRet = FALSE;
	bRet = WriteProcessMemory(processo, LPVOID(PARA_MINAS), lpShellcode, ShellcodeTam, &dwWritten);
	CloseHandle(processo);

	free(lpShellcode);

	//return(0);
}

void descongelarminas() {
	HANDLE processo = abrirProcesso();
	DWORD ShellcodeTam = 2; //bytes do shellcode
	LPBYTE lpShellcode = (LPBYTE)malloc(ShellcodeTam);
	DWORD escrito = 0;

	if (processo == NULL) {
		printf("Não achei a janela dessa porcaria");
		exit(1);
	}
	memcpy(lpShellcode, voltarverificacaominasShellcode, ShellcodeTam);

	DWORD dwWritten = 0;
	BOOL bRet = FALSE;
	bRet = WriteProcessMemory(processo, LPVOID(PARA_MINAS), lpShellcode, ShellcodeTam, &dwWritten);
	CloseHandle(processo);

	free(lpShellcode);

	//return(0);
}



void mostrarminas() {
	HANDLE processo = abrirProcesso();
	BOOL bRet = FALSE;
	BOOL sRet = FALSE;
	unsigned long int tamanho = END_MINES - BEG_MINES;
	unsigned int TamanhoStatus = sizeof(int);
	int *pnt = NULL;
	LPVOID bufferstatus = malloc(TamanhoStatus);
	LPVOID buffer = malloc(tamanho);
	sRet = ReadProcessMemory(processo, LPVOID(STATUS), bufferstatus, TamanhoStatus, NULL);
	if (sRet != 0) {
		pnt = (int *) bufferstatus;
	}
	while (*pnt==0) {
		bRet = ReadProcessMemory(processo, LPVOID(BEG_MINES), buffer, tamanho, NULL);
		if (bRet != 0) {
			//printf("Leu!");
			//printf("%s\n", buffer);
			imprimirminas(buffer);
		}
		Sleep(1000);
		system("cls");
		sRet = ReadProcessMemory(processo, LPVOID(STATUS), bufferstatus, TamanhoStatus, NULL);
	}
}

void imprimirminas(LPVOID buffer) {
	int i, cont=0;
	char *iniBuffer, *point;
	iniBuffer = (char *) buffer;
	point = iniBuffer;
	for (i = BEG_MINES; i <= END_MINES; i++) {
		//printf("%X ", *point);
		//point++;
		if (*point == 0x10) {
			printf(" t ");
		}
		else if (*point == 0x0F) {
			printf(" - ");
		}
		else if (*point == 0xFFFFFF8F) {
			printf(" x ");
		}
		else if (*point == 0xFFFFFF8E) {
			printf(" B ");
		}
		else if (*point == 0x40) {
			printf(" @ ");
		}
		else if (*point == 0x41) {
			printf(" 1 ");
		}
		else if (*point == 0x42) {
			printf(" 2 ");
		}
		else if (*point == 0x43) {
			printf(" 3 ");
		}
		else if (*point == 0x44) {
			printf(" 4 ");
		}
		else if (*point == 0x45) {
			printf(" 5 ");
		}
		else if (*point == 0x46) {
			printf(" 6 ");
		}
		else if (*point == 0x47) {
			printf(" 7 ");
		}
		else if (*point == 0x48) {
			printf(" 8 ");
		}
		else printf(" %X ", *point);

		if (cont == 31) {
			printf("\n");
			cont = -1;
		}
		point++;
		cont++;
	}
}

HANDLE abrirProcesso() {
	DWORD ProcessoId = 0;
	HWND janela = FindWindow(NULL, "MineSweeper");
	if (janela == NULL) {
		printf("Não achei a janela\n");
		return NULL;
	}

	GetWindowThreadProcessId(janela, &ProcessoId);
	return OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_VM_OPERATION, FALSE, ProcessoId);
}
