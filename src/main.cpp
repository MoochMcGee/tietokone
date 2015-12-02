#include "common.h"
#include "mem.h"
#include "cpu.h"
#include "commands.h"

std::vector<std::string> splitStrn(const char *str, const char *delim);
char *input(const char *prompt);

int main(int ac, char** av)
{
    FILE* biosfp = fopen("roms/bios.bin","rb");

    fread(bios,0x20000,1,biosfp);

    fclose(biosfp);

    log_filter = error | warning | debug | verbose | info;

    handlers.push_back(bioshandler);

    cpu maincpu;


	printf("Tietokone 386 PC Emulator by the Tietokone Team\n");
	Command::loadCLI();

	new Command("start", "Start the emulated processor", [&] (std::vector<std::string> args)
	{
		maincpu.init();
	});


	new Command("run", "Run the emulated processor", [&] (std::vector<std::string> args)
	{
		for(int i = 0;i<50;i++)
		{
			maincpu.tick();
		}
	});

	new Command("step", "Single-step the emulated processor", [&] (std::vector<std::string> args)
	{
		maincpu.tick();
	});


	char *prompt;
	printf("Use \"hlp\" to see the list of available commands.\n");
	do
	{
		prompt = input("> ");
		std::vector<std::string> data = splitStrn(prompt, " ");
		if(data.size() > 0) {               // ensure we don't cause an index out of bounds exception.
            for(int i = 0; i < (signed) Command::myCommands.size(); i++)
			{
                if(Command::myCommands.at(i).isThis(data.at(0)))
				{
                    std::vector<std::string> sub(data.begin() + 1, data.end());
                    Command::myCommands.at(i).doCommand(sub);
                }
            }
        }
	}
	while(true);
}

char *input(const char *prompt)
{
	int size = 256;											// we will allocate 256 chars for this purpose.
	char * data = (char *) malloc(size * sizeof(char));		// size * 1 byte. (platform dependent). May need to be replaced with wchar later.
	printf("%s", prompt);									// display the prompt...
	std::cin.getline(data, size);							// read in to data.
	if(std::cin.fail() || std::cin.bad())
	{
		printf("input stream error; resetting.\n");
		std::cin.clear();										// clear the stream error flags
		std::cin.ignore(INT_MAX, '\n');							// remove any newlines in the stream.
	}
	return data;
}

/**
 * Tokenizes a string according to a specified delimiter.
 *
 * @author ryanbwork@stackoverflow
 * @link http://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
 *
 * Special thanks to reengine for helping debug this function.
 */
std::vector<std::string> splitStrn(const char *str, const char *delim)
{
    std::string myData(str);
    std::string token;
    std::vector<std::string> collection;
    while ( token != myData)
	{
        token = myData.substr(0, myData.find_first_of(delim));
        myData = myData.substr(myData.find_first_of(delim) + 1);
        collection.push_back(token);
    }
    return collection;
}
