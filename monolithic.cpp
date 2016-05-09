/*

PROTECTED BY CREATIVE COMMONS SHARE ALIKE LICENCE - 
http://creativecommons.org/licenses/by-sa/3.0/

DO NOT REMOVE THIS COMMENT (unless you are copying the code for some academic project ;)

created by - apoorv kumar
iit guwahati
apoorv.kumar247@gmail.com

do visit my blog for more codes and computer stuff- 
http://eat-sleep-code.blogspot.com/

*/
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

//global ctrl params

struct Sdecoded_instruction
{
    string type;
    string sop1;
    string sop2;
    string sop3;
};

//var format in the mem
struct Svariable
{
    string var_name;
    int var_val;
};


//making the params global currently , but should be specific to certain modules.

class MipsGlobalCtrlParams
{
    public:
    static Sdecoded_instruction current_inst;

    //memory access bus
    static string var_name;
    static int var_val;

    //request to mem from op_fetch
    static bool read;
    static bool write;
    static bool define;

    //op_fetch to reg_file
    static bool write_to_reg_file; // from the memory bus - var_val
    static string reg_index;

};

// -----------------  instances of static vars ----------------
    Sdecoded_instruction MipsGlobalCtrlParams::current_inst;

    //memory access bus
    string MipsGlobalCtrlParams::var_name;
    int MipsGlobalCtrlParams::var_val;

    //request to mem from op_fetch
    bool MipsGlobalCtrlParams::read;
    bool MipsGlobalCtrlParams::write;
    bool MipsGlobalCtrlParams::define;

    //op_fetch to reg_file
    bool MipsGlobalCtrlParams::write_to_reg_file; // from the memory bus - var_val
    string MipsGlobalCtrlParams::reg_index;

// -------------------------------------------------------------



class Cwrite_back
{
    public:
    static void write_back();

    private:
    //nothing
};





class Cinstruction_decoder
{
    public:
    static void decode(string instruction);

    private:
    //nothing - class is purely functional

};





class Cregister_file
{
    public:
    static void print_register_file();
    static int get_register(string index);// index of the format $0 , $1 .. .etc
    static void set_register(string index, int value);
    static int get_register(int index);// index of the format 0 , 1  .. .etc
    static void set_register(int index, int value);

    private:

    static int register_file[32];
};


// -----------------  instances of static vars ----------------
    int Cregister_file::register_file[32];
// -------------------------------------------------------------




class Ccode_text
{

    public:
    static void load_code_file(string file_address);
    static void print_code_text();
    static string get_instruction_at_pc();
    static void update_pc(string location);
    static void update_pc(int offset);
    static int get_pc();
    static void call_error();

    private:
    static int pc;
    static vector<string> code_text;

};


// -----------------  instances of static vars ----------------
    int Ccode_text::pc;
    vector<string> Ccode_text::code_text;
// -------------------------------------------------------------






class Cop_fetch
{
    public:
    static void put_on_bus();
    static void get_from_bus();
    static void clear_bus();
    private:
    //nothing - purely functional
};





class Cinstruction_fetcher
{
    public:
    static string fetch_instruction();

    private:
    static bool valid();
    //no data - purely functional
};





class Cexecute
{
    public:
    static void execute();

    private:
    //nothing  - purely functional
};



class Cmemory
{
    public:
    static void syscall(); // the only interface ... akin to the mem controller
    static void print_memory();

    private:
    static int load_var(string var_name);
    static void save_var(string var_name , int var_val);
    static void define_var(string var_name , int var_val);

    static vector<Svariable> variable_array;

};

// -----------------  instances of static vars ----------------
    vector<Svariable> Cmemory::variable_array;
// -------------------------------------------------------------






//\ global ctrl params

// write back


void Cwrite_back::write_back()
{
    if( MipsGlobalCtrlParams::current_inst.type == "sw" )
    {
        cout << "writing back ... \n";
        //put the data on bus
        MipsGlobalCtrlParams::var_name = MipsGlobalCtrlParams::current_inst.sop2;
        MipsGlobalCtrlParams::var_val = Cregister_file::get_register(MipsGlobalCtrlParams::current_inst.sop1);

        //enable the signals
        MipsGlobalCtrlParams::write = true;
        MipsGlobalCtrlParams::read = false;
        MipsGlobalCtrlParams::define = false;
    }
    else if( MipsGlobalCtrlParams::current_inst.type == ".word" )
    {
        cout << "allocating space ... \n";
        //put the data on bus
        MipsGlobalCtrlParams::var_name = MipsGlobalCtrlParams::current_inst.sop1;
        MipsGlobalCtrlParams::var_val = atoi(MipsGlobalCtrlParams::current_inst.sop2.c_str());

        //cout << "put online:  " << MipsGlobalCtrlParams::var_name << " : " << MipsGlobalCtrlParams::current_inst.sop2 << endl ;
        //enable the signals
        MipsGlobalCtrlParams::write = false;
        MipsGlobalCtrlParams::read = false;
        MipsGlobalCtrlParams::define = true;
    }
}


//\ write back


// inst decode

void Cinstruction_decoder::decode(string instruction)
{
    Sdecoded_instruction dec_inst;

    istringstream instruction_stream(instruction , istringstream::in) ;
    //instruction_stream.str();

    string temp;


    //get instruction - skip whitespaces
    instruction_stream >> skipws >> temp;

    if( temp == "j" ) // 1 param
    {
        dec_inst.type = temp;
        instruction_stream >> dec_inst.sop1;
    }
    else if( temp == "lw" || temp == "sw" || temp == "li" || temp == ".word") // 2 params
    {
        string comma;
        dec_inst.type = temp;
        instruction_stream >> dec_inst.sop1 >> comma >> dec_inst.sop2 ; // get operands - regid , var_name

    }
    else if( temp == "beq" || temp == "add" || temp == "sub" || temp == "or" || temp == "slt" || temp == "and" ) // 3 params
    {
        string comma;
        dec_inst.type = temp;
        instruction_stream >> dec_inst.sop1 >> comma >> dec_inst.sop2 >> comma >> dec_inst.sop3;
    }
    else if( temp == "exit") // 0 params
    {
        dec_inst.type = temp;
    }

    else
    {
        cerr << "error: THE INSTRUCTION " << temp << "COULDN'T BE IDENTIFIED\n";
    }

    MipsGlobalCtrlParams::current_inst =  dec_inst;
}

//\ inst decode


// register file



void Cregister_file:: print_register_file()
{
    cout << "register states:\n";
    for (int i = 0 ; i < 32 ; i++)
    {
        cout << "R[" << i << "] = " << register_file[i] << endl;
    }

    cout << " --------------------------   \n\n";
}

int Cregister_file:: get_register( string index)
{
    string striped_index = index.substr(1); // strip the $ sign
    int integer_index = atoi(striped_index.c_str()); // convert to int
    return register_file[integer_index];
}

int Cregister_file:: get_register( int index)
{
    return register_file[index];
}
void Cregister_file:: set_register(string index , int value)
{
    string striped_index = index.substr(1); // strip the $ sign
    int integer_index = atoi(striped_index.c_str()); // convert to int
    register_file[integer_index] = value;
}

void Cregister_file:: set_register(int index , int value)
{
    register_file[index] = value;
}

//\ register file


// code text


void Ccode_text:: load_code_file(string file_address)
{

	cout << "loading file - " << file_address << " ... \n";
    //convert str to char*
    fstream code_file( (char*)file_address.c_str() );

    string temp;

    while(getline(code_file, temp))
    {
        if(temp != "" && temp[0] != '#') code_text.push_back(temp); //if not a blank line or comment then push
    }

    if(code_text.size() == 0)
    {
        cout << "THE FILE IS EMPTY !!! nothing to execute\n";
        pc = 0;
    }
    else
    {
        pc = 1; // set pc to first line
    }

}

void Ccode_text::print_code_text()
{
    cout << "CODE TEXT STARTS-- " << endl;
    for(int i = 0 ; i < code_text.size() ; i++ )
    {
        cout << code_text[i] << endl;
    }
    cout << "--CODE TEXT ENDS " << endl;
}

string Ccode_text:: get_instruction_at_pc()
{
    return code_text[pc-1];
}

void Ccode_text::update_pc(string block_name)
{
    block_name = block_name + ":" ; // give it the <block_name><:> format

    for(int i =0 ; i < code_text.size() ; i++)
    {
        if(  !block_name.compare( code_text[i] ) ) // incase strings match
        {
            pc = i + 2; // +1 for making it index format , +1 for going to the next line
        }
    }

}

void Ccode_text::update_pc(int offset = 1) // bydefault , the single jump
{
    pc = pc + 1;
}

int Ccode_text:: get_pc()
{
    return pc;
}

void Ccode_text:: call_error()
{
    cout << "ERROR PARSING INSTRUCTION " << pc << " --- " << code_text[pc] << "---" << endl ;
}

//\ code text


// inst fetch




//returns inst after check
string Cinstruction_fetcher:: fetch_instruction()
{
    if( valid() )
    {
        //update pc - can be overridden later on by exec module
        string current_instruction = Ccode_text::get_instruction_at_pc();
        Ccode_text::update_pc();
        if(current_instruction[current_instruction.size()-1] == ':')
        {
            //skip if it is a block name and not an instruction
            current_instruction = Ccode_text::get_instruction_at_pc();
        }
        cout << "fetched instruction: " << current_instruction << endl;
        return current_instruction;

    }
    else
    {
        Ccode_text::call_error();
        string blank = "";
        return blank;
    }
}

//check
bool Cinstruction_fetcher:: valid()
{
    //validation code
    //to be implemented
    return 1;
}


//\ inst fetch



// op fetch

void Cop_fetch::clear_bus()
{
        MipsGlobalCtrlParams::read = false;
        MipsGlobalCtrlParams::write = false;
        MipsGlobalCtrlParams::define = false;
}

void Cop_fetch::put_on_bus()
{
    if(MipsGlobalCtrlParams::current_inst.type == "lw")
    {
        //place the var_name on the bus
        MipsGlobalCtrlParams::var_name = MipsGlobalCtrlParams::current_inst.sop2;
        //save the reg_index for future lookup
        MipsGlobalCtrlParams::reg_index = MipsGlobalCtrlParams::current_inst.sop1;
        //activate the read line
        MipsGlobalCtrlParams::read = true;
        MipsGlobalCtrlParams::write = false;
        MipsGlobalCtrlParams::define = false;
    }

}

void Cop_fetch::get_from_bus()
{
    //memory module must have put the data online by now ...
    if(MipsGlobalCtrlParams::current_inst.type == "lw")
    {
        //place the var_name on the bus
        string reg = MipsGlobalCtrlParams::current_inst.sop1;
        Cregister_file::set_register(reg , MipsGlobalCtrlParams::var_val);
    }

}



//\ op fetch


// memory


void Cmemory::syscall()
{

    if(MipsGlobalCtrlParams::read)
    {
        cout << "making syscall : read... \n";
        //var_name has been put on bus
        string var_name = MipsGlobalCtrlParams::var_name;
        //write the corresponding value on bus
        MipsGlobalCtrlParams::var_val = load_var(var_name);


        //clear bus status
        Cop_fetch::clear_bus();
    }
    else if(MipsGlobalCtrlParams::write)
    {
        cout << "making syscall : write... \n";
        //var_name has been put on bus
        string var_name = MipsGlobalCtrlParams::var_name;
        //var_val has been put on bus
        int var_val = MipsGlobalCtrlParams::var_val;
        save_var(var_name , var_val);


        //clear bus status
        Cop_fetch::clear_bus();
    }
    else if(MipsGlobalCtrlParams::define)
    {
        cout << "making syscall : define... \n";
         //var_name has been put on bus
        string var_name = MipsGlobalCtrlParams::var_name;
        //var_val has been put on bus
        int var_val = MipsGlobalCtrlParams::var_val;
        define_var(var_name , var_val);


        //clear bus status
        Cop_fetch::clear_bus();
    }

}

int Cmemory::load_var(string var_name)
{
    bool found = false;
    for (int i = 0 ; i < variable_array.size() ; i++)
    {
        if (var_name == variable_array[i].var_name)
        {
            return variable_array[i].var_val;
            found = true;
        }
    }
    if(!found)
    {
        cerr << "THE VARIABLE " << var_name << " NOT FOUND IN MEMORY\n";
        return 0;
    }

}

void Cmemory::define_var(string var_name , int var_val)
{
    Svariable temp;

    temp.var_name = var_name;
    temp.var_val = var_val;

    variable_array.push_back(temp);
}


void Cmemory::save_var(string var_name , int var_val)
{
    bool found = false;
    for (int i = 0 ; i < variable_array.size() ; i++)
    {
        if (var_name == variable_array[i].var_name)
        {
            variable_array[i].var_val = var_val;
            found = true;
        }
    }
    if(!found)
    {
        cerr << "THE VARIABLE " << var_name << " NOT FOUND IN MEMORY\n";
    }

}


void Cmemory::print_memory()
{
    cout << "MEMORY STATUS ------------\n";
    for(int i = 0 ; i < variable_array.size() ; i++)
    {
        cout << variable_array[i].var_name << " : " << variable_array[i].var_val << endl;
    }
    cout << "----------------------------\n";
}
//\ memory


// execute



void Cexecute::execute()
{

    string instruction = MipsGlobalCtrlParams::current_inst.type;
    if(instruction == "add")
    {
        cout << "adding ... \n";
        //get the reg names
        string dest_reg = MipsGlobalCtrlParams::current_inst.sop1;
        string op1_reg = MipsGlobalCtrlParams::current_inst.sop2;
        string op2_reg = MipsGlobalCtrlParams::current_inst.sop3;

        //get the reg vals
        int op1_val = Cregister_file::get_register(op1_reg);
        int op2_val = Cregister_file::get_register(op2_reg);

        //calculate result
        int result = op1_val + op2_val;

        //write back to reg
        Cregister_file::set_register(dest_reg , result);


    }

    else if(instruction == "sub")
    {
        cout << "subtracting ... \n";
        //get the reg names
        string dest_reg = MipsGlobalCtrlParams::current_inst.sop1;
        string op1_reg = MipsGlobalCtrlParams::current_inst.sop2;
        string op2_reg = MipsGlobalCtrlParams::current_inst.sop3;

        //get the reg vals
        int op1_val = Cregister_file::get_register(op1_reg);
        int op2_val = Cregister_file::get_register(op2_reg);

        //calculate result
        int result = op1_val - op2_val;

        //write back to reg
        Cregister_file::set_register(dest_reg , result);
    }

    else if(instruction == "slt")
    {
        cout << "setting less than \n";
        //get the reg names
        string dest_reg = MipsGlobalCtrlParams::current_inst.sop1;
        string op1_reg = MipsGlobalCtrlParams::current_inst.sop2;
        string op2_reg = MipsGlobalCtrlParams::current_inst.sop3;

        //get the reg vals
        int op1_val = Cregister_file::get_register(op1_reg);
        int op2_val = Cregister_file::get_register(op2_reg);

        //calculate result
         int result = (op1_val < op2_val)?op1_val:op2_val ; // set the min one

        //write back to reg
        Cregister_file::set_register(dest_reg , result);

    }

    else if(instruction == "and")
    {
        cout << "bitwise and ... \n";
        //get the reg names
        string dest_reg = MipsGlobalCtrlParams::current_inst.sop1;
        string op1_reg = MipsGlobalCtrlParams::current_inst.sop2;
        string op2_reg = MipsGlobalCtrlParams::current_inst.sop3;

        //get the reg vals
        int op1_val = Cregister_file::get_register(op1_reg);
        int op2_val = Cregister_file::get_register(op2_reg);

        //calculate result
        int result = op1_val & op2_val;// bitwise and

        //write back to reg
        Cregister_file::set_register(dest_reg , result);
    }

    else if(instruction == "or")
    {
        cout << "bitwise or ... \n";
        //get the reg names
        string dest_reg = MipsGlobalCtrlParams::current_inst.sop1;
        string op1_reg = MipsGlobalCtrlParams::current_inst.sop2;
        string op2_reg = MipsGlobalCtrlParams::current_inst.sop3;


        //get the reg vals
        int op1_val = Cregister_file::get_register(op1_reg);
        int op2_val = Cregister_file::get_register(op2_reg);

        //calculate result
        int result = op1_val|op2_val;// bitwise or

        //write back to reg
        Cregister_file::set_register(dest_reg , result);
    }


    else if(instruction == "j") // 1 param
    {
        cout << "jump ... \n";
        //get the jmp address
        string jump_address = MipsGlobalCtrlParams::current_inst.sop1;

        //call for jump
        Ccode_text::update_pc(jump_address);
    }

    else if(instruction == "beq")
    {

        //get the reg names
        string op1_reg = MipsGlobalCtrlParams::current_inst.sop1;
        string op2_reg = MipsGlobalCtrlParams::current_inst.sop2;
        string jump_address = MipsGlobalCtrlParams::current_inst.sop3;

        //get the reg vals
        int op1_val = Cregister_file::get_register(op1_reg);
        int op2_val = Cregister_file::get_register(op2_reg);

        //compare results and jump if needed
        if(op1_val == op2_val)
        {
            cout << "branching ... \n";
            Ccode_text::update_pc(jump_address);
        }
        else
        {
            //do nothing ...
        }

    }
    else if(instruction == "li")
    {
        string reg_address = MipsGlobalCtrlParams::current_inst.sop1;
        string reg_val = MipsGlobalCtrlParams::current_inst.sop2;
        Cregister_file::set_register(reg_address , atoi(reg_val.c_str() ) );
    }
}



//\ execute


// model


class MIPS_processor
{
    public:
    static void start_machine();
    static void print_code_text();
    static void print_register_file();
    static void terminate_machine();

    private:
    static string code_file_name;
    static void boot();
    static void run();

};

// -----------------  instances of static vars ----------------
    string MIPS_processor::code_file_name;
// -------------------------------------------------------------





void MIPS_processor::print_code_text()
{
    Ccode_text::print_code_text();
}

void MIPS_processor::print_register_file()
{
    Cregister_file::print_register_file();
}

void MIPS_processor::terminate_machine()
{
    cout << "shutting down machine ...\n";
    cout << "GOOD BYE  ..... \n";
}
void MIPS_processor::start_machine()
{
    cout << "WELCOME TO THE MIPS SIMULATOR: - designed by **apoorv kumar**\n";
    cout << "ENTER THE FILE NAME OF CODE YOU WANT TO EXECUTE: ";
    cin >> code_file_name;

    boot();
    run();


    //when exit
    terminate_machine();
}

void MIPS_processor::boot()
{
    cout << "booting machine... \n";

    //load file
    Ccode_text::load_code_file(code_file_name);

    //init params
    MipsGlobalCtrlParams::read = false;
    MipsGlobalCtrlParams::write = false;
    MipsGlobalCtrlParams::define = false;

        for (int i =0 ; i < 32 ; i++)
    {

        Cregister_file::set_register(i , 0);
    }


}

void MIPS_processor::run()
{
    cout << "starting ... \n";
    MipsGlobalCtrlParams::current_inst.type = "starting";


    print_code_text();

    while(MipsGlobalCtrlParams::current_inst.type != "exit")
    {
        cout << "*********** instruction exec started ************* \n";

    //inst fetch
        string instruction = Cinstruction_fetcher::fetch_instruction();

    //inst decode
        Cinstruction_decoder::decode(instruction);

    //op fetch
        //put varnames on bus
        Cop_fetch::put_on_bus();

        //call memory controller
        Cmemory::syscall();

        //get varval from bus
        Cop_fetch::get_from_bus();

        //clear bus status
        Cop_fetch::clear_bus();

    //execute
        Cexecute::execute();

    //write back
        Cwrite_back::write_back();

        //call memory controller
        Cmemory::syscall();


        //print params

        print_register_file();

        Cmemory::print_memory();


        cout << endl << "*********** instruction exec complete ************* \n" << endl;


    }
}





//\ model

int main()
{
    MIPS_processor::start_machine();
}

