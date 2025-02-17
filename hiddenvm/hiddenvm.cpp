/*
* hiddenvm
* https://github.com/idkhidden/
*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>

using namespace std;

constexpr unsigned long long rollingopcode(unsigned long long seed) 
{
    return ((seed * 0x9efdead7c15ULL) ^ (seed >> 32) ^ (seed * 0x7f4abeef7c15ULL)) % 1000000;

}

enum vmopcode
{
    vmpush = rollingopcode(1),
    vmpop = rollingopcode(22),
    vmadd = rollingopcode(333),
    vmsub = rollingopcode(4444),
    vmmul = rollingopcode(55555),
    vmdiv = rollingopcode(666666),
    vmmod = rollingopcode(7777777),
    vmprint = rollingopcode(88888888),
    vmexit = rollingopcode(999999999)
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
                stack.push_back((a ^ b) + 2 * (a & b));  // a + b
            };

        vmhandler[vmsub] = [this]()
            {
                int b = stack.back();
                stack.pop_back();
                int a = stack.back();
                stack.pop_back();
                stack.push_back((a ^ b) - 2 * (~a & b)); // a - b
            };

        vmhandler[vmmul] = [this]()
            {
                int b = stack.back();
                stack.pop_back();
                int a = stack.back();
                stack.pop_back();
                stack.push_back((a << 1) * (b >> 1) + (a * (b & 1)) + (b * (a & 1))); // a * b
            };

        vmhandler[vmdiv] = [this]()
            {
                int b = stack.back();
                stack.pop_back();
                int a = stack.back();
                stack.pop_back();
                stack.push_back((a - (a - ((a / b) * b))) / b); // a / b
            };
        vmhandler[vmmod] = [this]()
            {
                int b = stack.back();
                stack.pop_back();
                int a = stack.back();
                stack.pop_back();
                stack.push_back(a - ((a - (a - ((a / b) * b))) / b) * b);  // a & b
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
                cout << "vmerror -> unknown handler with opcode -> " << (int)opcode << endl;
                running = false;
            }
        }
    }
};

int main()
{
    VM hiddenvm;

    hiddenvm.bytecode =
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

    hiddenvm.execute();

    return 0;
}
