#include <iostream>
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include <functional>

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
    vmexit = 32
};

class VM
{
public:
    vector<uint64_t> stack;
    vector<uint64_t> bytecode;
    size_t instructionpointer = 0;
    bool running = true;
    unordered_map<uint8_t, function<void()>> jumptable;

    VM()
    {
        jumptable[vmpush] = [this]()
            {
                int value = bytecode[instructionpointer++];
                stack.push_back(value);
            };

        jumptable[vmpop] = [this]()
            {
                if (!stack.empty())
                {
                    stack.pop_back();
                }
            };

        jumptable[vmadd] = [this]()
            {
                int b = stack.back(); 
                stack.pop_back();
                int a = stack.back();
                stack.pop_back();
                stack.push_back(a + b);
            };

        jumptable[vmsub] = [this]()
            {
                int b = stack.back(); 
                stack.pop_back();
                int a = stack.back(); 
                stack.pop_back();
                stack.push_back(a - b);
            };

        jumptable[vmmul] = [this]()
            {
                int b = stack.back(); 
                stack.pop_back();
                int a = stack.back(); 
                stack.pop_back();
                stack.push_back(a * b);
            };

        jumptable[vmdiv] = [this]()
            {
                int b = stack.back(); 
                stack.pop_back();
                int a = stack.back(); 
                stack.pop_back();
                stack.push_back(a / b);
            };

        jumptable[vmprint] = [this]()
            {
                cout << "vmprint -> " << stack.back() << endl;
            };

        jumptable[vmexit] = [this]()
            {
                running = false;
            };
    }

    void execute()
    {
        while (running)
        {
            uint8_t opcode = bytecode[instructionpointer++];

            if (jumptable.find(opcode) != jumptable.end())
            {
                cout << "jumptable entry\n";
                jumptable[opcode]();
            }
            else
            {
                cout << "vmerror -> unknown opcode " << (int)opcode << endl;
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

    return 0;
}
