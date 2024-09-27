/*
* hiddenvm
* https://github.com/idkhidden/
*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>

using namespace std;

enum vmopcode
{
    vmpush = 0x764,
    vmpop = 0x235,
    vmadd = 0x001,
    vmsub = 0x342,
    vmmul = 0x573,
    vmdiv = 0x999,
    vmmod = 0x144,
    vmprint = 0x346,
    vmexit = 0x831
};

class VM
{
public:
    vector<uint64_t> stack;
    vector<uint64_t> bytecode; 
    size_t instructionpointer = 0;
    unordered_map < uint64_t, function<void()>> vmhandler;
    bool running = true;

    VM()
    {
        vmhandler[vmpush] = [this]()
            {
                uint64_t value = bytecode[instructionpointer++];
                stack.push_back(value);
            };

        vmhandler[vmpop] = [this]()
            {
                if (!stack.empty())
                {
                    stack.pop_back();
                }
            };

        vmhandler[vmadd] = [this]()
            {
                int b = stack.back();
                stack.pop_back();
                int a = stack.back();
                stack.pop_back();
                stack.push_back(a + b);
            };

        vmhandler[vmsub] = [this]()
            {
                int b = stack.back();
                stack.pop_back();
                int a = stack.back();
                stack.pop_back();
                stack.push_back(a - b);
            };

        vmhandler[vmmul] = [this]()
            {
                int b = stack.back();
                stack.pop_back();
                int a = stack.back();
                stack.pop_back();
                stack.push_back(a * b);
            };

        vmhandler[vmdiv] = [this]()
            {
                int b = stack.back();
                stack.pop_back();
                int a = stack.back();
                stack.pop_back();
                stack.push_back(a / b);
            };
        vmhandler[vmmod] = [this]()
            {
                int b = stack.back();
                stack.pop_back();
                int a = stack.back();
                stack.pop_back();
                stack.push_back(a % b);
            };

        vmhandler[vmprint] = [this]()
            {
                if (!stack.empty())
                {
                    cout << "vmprint -> " << stack.back() << endl;
                }
            };

        vmhandler[vmexit] = [this]()
            {
                running = false;
            };
    }

    void execute()
    {
        while (running && instructionpointer < bytecode.size())
        {
            uint64_t opcode = bytecode[instructionpointer++];

            if (vmhandler.find(opcode) != vmhandler.end())
            {
                vmhandler[opcode]();
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
