#include <iostream>
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
    vmmod,
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
                uint8_t value = bytecode[instructionpointer++];
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
        jumptable[vmmod] = [this]()
            {
                int b = stack.back();
                stack.pop_back();
                int a = stack.back();
                stack.pop_back();
                stack.push_back(a % b);
            };

        jumptable[vmprint] = [this]()
            {
                if (!stack.empty())
                {
                    cout << "vmprint -> " << stack.back() << endl;
                }
            };

        jumptable[vmexit] = [this]()
            {
                running = false;
            };
    }

    void execute()
    {
        while (running && instructionpointer < bytecode.size())
        {
            uint8_t opcode = bytecode[instructionpointer++];

            if (jumptable.find(opcode) != jumptable.end())
            {
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
        vmpush, 5,        
        vmpush, 10,     
        vmadd,           
        vmprint,
        vmpop,
        vmpush, 100,     
        vmpush, 20,    
        vmsub,            
        vmprint,
        vmpop,
        vmpush, 200,
        vmpush, 1337,
        vmmul,
        vmprint,
        vmpop,
        vmpush, 69,
        vmpush, 10,
        vmdiv,
        vmprint,
        vmpop,
        vmpush, 101,
        vmpush, 10,
        vmmod,
        vmprint,
        vmexit
    };

    cout << "vmexecute entry\n";
    vm.execute();

    return 0;
}
