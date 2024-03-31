## chainway_challenge
Chainway Senior Protocol Engineer Challenge

# BUILDING

-  Dependencies: gcc, g++, make
-  tpye make all
-  It will produce "chainway_app" file, then type "./chainway_app" to launch the application
-  type make clean to clean files


# MENU
-  The application has initied by "from_sequencer.txt" file which must be exist in the application path.
-  A command menu will pop up, which provides following commands.
    -  'p' -> It reads a line from "from_sequencer.txt," creates a block, and then publishes it to the batch. If the batch length is equal to 5, then the batch is pushed to DA and cleared.
    -  'b' -> It completes the current batch to 5 blocks, then pushes it to DA.
    -  'g' -> Returns a value and state of the given key, usage: g key. It first looks for within the current batch and then seeks in the DA layer if it does not exist.
    -  'h' -> Returns the value at block height for the given key, usage: h <key> <block>. It looks for the key within L2 blocks.
    -  'hall' -> Returns all history for the given key, usage: hall key. It seeks within the L2 blocks.
    -  1 -> REORG 1
    -  2 -> REORG 2
    -  3 -> REORG 3

# TEST

-  generator.py can be used to create new test cases.
-  Example usage: python3 generator.py

