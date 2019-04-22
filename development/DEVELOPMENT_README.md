# RLC Development

This is a guide to get you started with development for the RLC.

## Setting up the enviorenment

These instructions will get you a copy of the project up and running.

### Prerequisites

- Ardublock and OpenBlocks is built under java, so some java knowledge is recommended but not required.
- We develop under Visual Studio Code so you need to have it installed and know how to work with it, alternatively you can use eclipse or some other IDE, but you will be on your own.
- We develop under linux so everything in this tutorial will revolve around it, you can use windows or macOS but again, you will be on your own.
- Your VSCode should have the following extensions:

- you will need the following packages under linux:
    - maven

### Setup a fresh ArduBlock

these istructions will set you up with a fresh ardublock project, without any of the work we have done already.
You should use our version of ardublock in this repository, this has the blocks we made already, along with some bugfixing and the development enviorenment already set up.

- Clone the ArduBlock repository
```
git clone https://github.com/taweili/ardublock ardublock
```

- Install OpenBlocks Dependency
```
./ardublock/install_openblocks
```

- initial Setup
```
mvn validate
```

To compile:
```
mvn compile
```
To Run:
```
mvn exec:java -Dexec.mainClass="com.ardublock.Main"
```

this is all you need to start developing.
after compiling
under ./target/
there is a file called ardublocks-all.jar
this is the file you install in the arduino ide

### Setup VSCode

- Open VSCode i ardublocks directory
```
code ardublocks
```

- Install extensions, we recomend:


- Create a tasks.json and launch.json under .vscode, these are standard files and there are instructions in vscode and online, you can just copy ours as well.

## installing your compiled Ardublock

- Install the Arduino IDE if it is not already installed. instructions in [README](software/SOFTAWARE_README.md).

- Inside the Arduino installation Folder (the one you extracted before) you should be able to see a folder named "tools", open it.

- Create a folder named "ArduBlockTool" WARNING: THIS IS CASE SENSITVE.

- Inside the folder you just created create a new one named "tool".

- Copy ardublock-all.jar (the file you compiled) to the folder you just created (tool). ardublock-all.jar is located under ardublock/target/

- Now open the Arduino IDE and in the tab "Tools" you should see an option "ArduBlock" if everything went well.

## Creating new Blocks

These instructions will show you how to create new Blocks.

### Things you should know

- Nomenclatures:
Block: lowest level module, what actually is used to programm and generates the code
Socket: where the blocks connect, there are various types of socket, data, number, etc.
Drawer: contains the blocks

- You should only ever need to modify the following existing files:
ardublock/src/main/resources/com/ardublock/block/ardublock.properties - contains the names and descriptions of the blocks
ardublock/src/main/resources/com/ardublock/block/ardublock.xml - this is where you describe the blocks, type, sockets, also where you create the drawer and associate blocks with certain drawers
ardublock/src/main/resources/com/ardublock/block/block-mapping.properties - this is where you declare a block
and any classes you create

### Creating your own Blocks

- Ceate a directory in ardublock/src/main/java/com/ardublock/translator/block where you will put your classes / Blocks

- Create a new Class file that will become your class: BLOCK_NAME.java

- Inside this file you will write:
```
package com.ardublock.translator.block.THE_DIR_YOU_CREATED;

import com.ardublock.translator.Translator;
import com.ardublock.translator.block.TranslatorBlock;
import com.ardublock.translator.block.exception.SocketNullException;
import com.ardublock.translator.block.exception.SubroutineNotDeclaredException;

public class BLOCK_NAME extends TranslatorBlock
{
	public blockname(Long blockId, Translator translator, String codePrefix, String codeSuffix, String label)
	{
		super(blockId, translator, codePrefix, codeSuffix, label);
	}

	@Override
	public String toCode() throws SocketNullException,  SubroutineNotDeclaredException
	{
		THIS IS WHERE YOU WRITE THE BLOCK
	}

}
```

- The following calls are used to create blocks:
    - add a string(funcion call defenition etc) to setup()
        ```
        translator.addSetupCommand("foo_init();");
        ```

    - create a socket, this is where other blocks conect to, optional
        ```
        TranslatorBlock Socket0 = this.getRequiredTranslatorBlockAtSocket(0);
        String Socket0s = Socket0.toCode();
        ```

    - add header file, self explanatory
        ```
        translator.addHeaderFile("ma_head.h");
        ```

    - create a macro or define a funcion
        ```
        translator.addDefinitionCommand("#define THIS()\t(" + Socket0s + ")";
        ```

        ```
        translator.addDefinitionCommand(
				"int test_func(int var)\n" +
                "{\n" +
				"	return var;" +
				"}\n" );
        ```

    - code that will be written to main loop, or to where this block was called
        this corresponds to the string that is returned
        ```
        String ret = "test_func(" + Socket0s + ");";
        return ret;
        ```

- Now you need to declare this block so it can be called in block-mapping.properties (add to end of file)
```
BLOCK_NAME_1=com.ardublock.translator.block.iplbot.BLOCK_CLASS_NAME
```

- Now you need to define this block's properties in ardublock.properties (add to end of fil-e)
```
bd.DRAWER_NAME=drawer name

bg.BLOCK=BLOCK_NAME_1
bg.BLOCK.description=description, tooltip
bg.BLOCK_NAME_1_COMMAND=block name 1

bc.SOCKET1_LABEL=label1
bc.SOCKET2_LABEL=label2
```

- Define The blocks in ardublock.xml
- In the end of ardublock.xml file you create a drawer set
```
<BlockDrawer button-color="125 125 125" name="bd.BLOCK_DRAWER_NAME">
	<BlockGenusMember>BLOCK_NAME_1</BlockGenusMember>
    <BlockGenusMember>BLOCK_NAME_2</BlockGenusMember>
</BlockDrawer>
```

- around line 11070( right before "BlockGenuses" is closed) is where you define the blocks themselves
```
<BlockGenus name="BLOCK_NAME_1" kind="BLOCK_TYPE" color="69 69 69" initlabel="bg.BLOCK_COMAND">
    <description>
        <text>BLOCK DESCRIPTION</text>
    </description>
    <BlockConnectors>
    </BlockConnectors>
</BlockGenus>
```
```
<BlockGenus name="BLOCK_NAME_2" kind="command" color="69 69 69" initlabel="bg.wifi_connect_command">
    <description>
        <text>BLOCK DESCRIPTION</text>
    </description>
    <BlockConnectors>
        <BlockConnector connector-type="string" connector-kind="socket" label="bc.SOCKET1_LABEL">
            <DefaultArg genus-name="message" label="label1" />
        </BlockConnector>
        <BlockConnector connector-type="string" connector-kind="socket" label="bc.SOCKET2_LABEL">
            <DefaultArg genus-name="message" label="label2" />
        </BlockConnector>
    </BlockConnectors>
</BlockGenus>
```
