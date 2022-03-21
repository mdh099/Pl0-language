Authors:
	Jacob Cordonero
  Mathew Holden
  Gabriella On-Cuen

No modifications to the package outside of vm.c

Modificaitons to vm.c skeleton
  We only made 3 small changes to the skeleton of vm.c. First we made a void function that is called for each fetch cycle, and just fills our IR struct with the information of code[PC]
  The second change we made was was adding a struct to represent the Information register. The struct has a typedef so it can be referred to as IR instead of struct IR. 
  Since we created the struct IR as, 
                      typedef struct IR{} IR; 
  we changed the struct information parameter in print_execution function, to IR IR. Other than this, there 
  were no changes to the entire package. 

