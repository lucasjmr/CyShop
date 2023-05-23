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


##########################################################
# LES COMMANDES SUIVANTES MARCHENT UNIQUEMENT SOUS LINUX #
##########################################################

#Supprime l'executable, les fichiers objets, et les zip
clean :
	rm -f $(EXE) *.o *.zip

#Cree un zip avec le code source 
release : clean
	zip -r release-1.x.zip *