#include <iostream>
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include <random>


using namespace std;




enum vmopcode 
{
    vmpush = 25,
    vmpop = 44,  
    vmadd = 99,
    vmsub = 42,  
    vmmul = 24,  
    vmdiv = 12,  
    vmprint = 36,
    vmexit = 32,
};

class VM
{
public:
    vector<uint64_t> stack;
    vector<uint64_t> bytecode;
    size_t instructionpointer = 0;
    bool running = true;

    void execute()
    {
        while (running)
        {
            uint8_t opcode = bytecode[instructionpointer++];

            if (opcode == vmpush)
            {
                int value = bytecode[instructionpointer++];
                stack.push_back(value);
            }
            else if (opcode == vmpop)
            {
                if (stack.empty()) 
                {
                    stack.pop_back(); 
                }
            }
            else if (opcode == vmadd)
            {
                int b = stack.back(); stack.pop_back(); 
                int a = stack.back(); stack.pop_back(); 
                stack.push_back(a + b);
            }
            else if (opcode == vmsub)
            {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a - b);
            }
            else if (opcode == vmmul)
            {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a * b);
            }
            else if (opcode == vmdiv)
            {

                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a / b);
            }
            else if (opcode == vmprint)
            {
                cout << "vmprint -> " << stack.back() << endl;
            }
            else if (opcode == vmexit)
            {
                running = false;
            }
            else
            {
                cout << "vmerror -> unknown opcode" << (int)opcode << endl;
                running = false;
            }
        }
    }
    
};

int main() 
{
    VM vm;

    cout << "bytecode entry\n";
    vm.bytecode =
    {
        vmpush, 1337,   
        vmpush, 69,   
        vmdiv,         
        vmprint,      
        vmexit         
    };

   cout << "vmexecute entry\n";
   vm.execute();

}
