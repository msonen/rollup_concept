# Preliminary
A sovereign rollup is a type of blockchain that publishes its transactions to
another blockchain, typically for ordering and data availability but handles its
own settlement. Sovereign rollups are responsible for execution and settlement,
while the Data Availability (“DA”) layer handles consensus and data availability.
The DA layer doesn’t verify whether sovereign rollup transactions are correct.
Nodes verifying the sovereign rollup are responsible for verifying whether new
transactions are correct.
This rollup is a single sequencer rollup using Bitcoin as DA. Since the
time between Bitcoin blocks, on average, is 10 minutes, relying solely on the DA
for block (L2 blocks) propagation is infeasible. This creates the need for softcommitments,
trusted blocks published by the sequencer that can be validated
by full-nodes, however, are not guaranteed to be in the canonical chain until
they finalize on the DA layer.

# The Challenge
To keep things simple, for this challenge, the rollup’s purpose will be to keep a
record of a 256-leaf Merkle tree. The sequencer will publish 5 L2 blocks then it
will write the state changes from the 5 blocks to the DA layer. Assume a DA
block is finalized after 4 confirmations (a block is finalized after 3 other blocks
are mined on top of it).
The full nodes have to implement a storage mechanism that can separate blocks
and batches of blocks in three different states:
1. Trusted state: This is a block published by the sequencer on a peer-to-peer
basis. The sequencer might be propagating different blocks to L2 nodes
and the DA layer.
2. On DA, not finalized state: This is result of Merkle leaf changes from
a 5 L2 block batch that was included in a DA block however it can be
invalidated. It isn’t in a finalized block on the DA layer therefore if a reorg
happens on the DA layer the full nodes also have to roll back.
3. On DA, finalized state: This is a 5 L2 block batch that was included in a
finalized DA block.
The sequencer will publish different numbers of key-value pairs, keys are indices
of the merkle leaf and the values are integers to put inside the leaves. A block
might have more than one change to the same leaf, in this case, the last change
is put on the Merkle leaf. The full node implementation will read these blocks
from lines of the from_sequencer.txt file, every line represents an L2 block.
The DA layer will be represented by the from_da.txt file and every line will
include one of the following:
1. Arbitrary number of key-value pairs representing the diff from 5 block
batches.
2. The word REORG followed by a number from 1 to 3, representing a reorg
happening on the DA layer, meaning that the last [1, 3] state-changes are
now invalid and the next lines will be replacing the previous lines.
We want you to implement a Rust program (mock full node) that will read from
the from_sequencer.txt and the from_da.txt files with respect to 5 blocks
from the sequencer and 1 batch of blocks from the DA layer basis, keep track
of the blocks and batches and keep track of the merkle-tree after every state
change, the merkle-tree has to be the result of all the state changes from the
blocks and batches. You may use a library for the merkle-tree.

# Building

-  Dependencies: gcc, g++, make
-  tpye make all
-  It will produce "app" file, then type "./app" to launch the application
-  type make clean to clean files


# Menu
-  The application has initied by "from_sequencer.txt" file which must be exist in the application path.
-  A command menu will pop up, which provides following commands.
    -  'p' -> It reads a line from "from_sequencer.txt," creates a block, and then publishes it to the batch. If the batch length is equal to 5, then the batch is pushed to DA and cleared.
    -  'b' -> It completes the current batch to 5 blocks, then pushes it to DA.
    -  'g' -> Returns a value and state of the given key, usage: g key. It first looks for within the current batch and then seeks in the L2 layer if it does not exist.
    -  'h' -> Returns the value at block height for the given key, usage: h key <block>. It looks for the key within L2 blocks.
    -  'hall' -> Returns all history for the given key, usage: hall key. It seeks within the L2 blocks.
    -  1 -> REORG 1
    -  2 -> REORG 2
    -  3 -> REORG 3

# Test

-  generator.py can be used to create new test cases.
-  Example usage: python3 generator.py

