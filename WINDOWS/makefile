CC = gcc
EXE = program
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

#make all cree l'executable
all : $(EXE)

#creer les objets a partir des fichiers source 
input.o : input.c input.h
	$(CC) -c $< -o $@
main.o : main.c managecustomer.h
	$(CC) -c $< -o $@
managestock.o : managestock.c managestock.h
	$(CC) -c $< -o $@
managecustomer.o : managecustomer.c managecustomer.h
	$(CC) -c $< -o $@

#creer executable a partir des objets
$(EXE) : $(OBJ)
	$(CC) $^ -o $@


############################################################
# LES COMMANDES SUIVANTES MARCHENT UNIQUEMENT SOUS WINDOWS #
############################################################

#Supprime le .exe, les fichiers objets, et les zip
clean :
	del $(EXE).exe *.o *.zip

cleanvs :
	rmdir /s /q ".vscode"

#Cree un zip avec le code source --> 7-ZIP REQUIS <--
release : clean
	7z a release-1.x.zip *.* *\*
