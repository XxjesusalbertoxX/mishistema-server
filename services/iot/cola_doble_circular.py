class ColaDobleCircular:
    def __init__(self, limite=5):
        self.limite = limite
        self.cola = [0] * limite
        self.inicio = -1
        self.fin = -1
    
    def inicializar_borrar_cola(self):
        self.inicio = -1
        self.fin = -1
        print("Cola vacía.")
    
    def mostrar_cola(self):
        if self.inicio == -1:
            print("Cola Vacía. No se puede mostrar ni eliminar elementos.")
            return
        
        i = self.inicio
        while True:
            print(f"Posición {i}: {self.cola[i]}")
            i = (i + 1) % self.limite
            if i == (self.fin + 1) % self.limite:
                break
    
    def insertar(self):
        if (self.fin + 1) % self.limite == self.inicio:
            print("Cola llena, no se puede insertar.")
            return
        
        valor = int(input("Ingrese el número a insertar: "))
        
        if self.inicio == -1:
            opcion = input("¿Desea insertar por el inicio (I) o por el final (F)? ").upper()
            
            if opcion == 'I':
                self.inicio = self.limite - 1
                self.fin = self.limite - 1
                self.cola[self.inicio] = valor
            elif opcion == 'F':
                self.inicio = 0
                self.fin = 0
                self.cola[self.fin] = valor
            else:
                print("Opción inválida.")
        else:
            opcion = input("¿Desea insertar por el inicio (I) o por el final (F)? ").upper()
            
            if opcion == 'I':
                self.inicio = (self.inicio - 1 + self.limite) % self.limite
                self.cola[self.inicio] = valor
            elif opcion == 'F':
                self.fin = (self.fin + 1) % self.limite
                self.cola[self.fin] = valor
            else:
                print("Opción inválida.")
    
    def eliminar(self):
        if self.inicio == -1:
            print("Cola Vacía. No se puede eliminar nada.")
            return
        
        opcion = input("¿Desea eliminar por el inicio (I) o por el final (F)? ").upper()
        
        if opcion == 'I':
            print(f"Elemento eliminado: {self.cola[self.inicio]}")
            if self.inicio == self.fin:
                self.inicio = -1
                self.fin = -1
            else:
                self.inicio = (self.inicio + 1) % self.limite
        elif opcion == 'F':
            print(f"Elemento eliminado: {self.cola[self.fin]}")
            if self.inicio == self.fin:
                self.inicio = -1
                self.fin = -1
            else:
                self.fin = (self.fin - 1 + self.limite) % self.limite
        else:
            print("Opción inválida.")
    
    def mostrar_creditos(self):
        print("- Creditos:")
        print("- Jesus Alberto Villarreal Perez 24170021")
        print("- Sebastian Rodriguez Contreras 😐No me se su matrilcula")
    
    def continuar(self):
        input("\nPresione Enter para continuar...")
    
    def mostrar_menu(self):
        while True:
            print("\nMenú de opciones:")
            print("1) Inicializar o Borrar Cola")
            print("2) Mostrar Cola")
            print("3) Insertar")
            print("4) Eliminar")
            print("5) Créditos")
            print("6) Salir")
            opcion = input("Escribe una opcion: ")
            
            if opcion == "1":
                self.inicializar_borrar_cola()
            elif opcion == "2":
                self.mostrar_cola()
            elif opcion == "3":
                self.insertar()
            elif opcion == "4":
                self.eliminar()
            elif opcion == "5":
                self.mostrar_creditos()
            elif opcion == "6":
                return
            else:
                print("Opción no válida.")
            
            self.continuar()


if __name__ == "__main__":
    cola = ColaDobleCircular()
    cola.mostrar_menu()

