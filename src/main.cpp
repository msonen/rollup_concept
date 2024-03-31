#include "smt.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include "sequencer.h"
#include "block.h"

using namespace std;

#define BATCH_LENGTH		5
#define MINER_COUNT			3


typedef enum
{
	CMD_PUB,
	CMD_BATCH,
	CMD_GET,
	CMD_HISTORY,
	CMD_REORG,
	CMD_HISTORY_ALL

}cmd_t;

static vector<block*> L2;
static vector<block*> L1;

static string state2str(enum FinalizationStatus stat)
{
	switch (stat)
	{
		case Trusted:
			return string("Trusted");
		case DaFinalized:
			return string("DaFinalized");
		case DaNotFinalized:
			return string("DaNotFinalized");
	}

	return string("Unknown");
}

static void print_block(block* block)
{
	cout << "[";

	for(auto& pair : block->data)
	{
		cout << "{" << pair.first << ", " << pair.second << "}, ";
	}
	cout << "]" << endl;
}

static seq_ret_t publish_block(FILE* fp, vector<block*>& batch)
{
	block *blck = new block;
	block_init(blck);
	seq_ret_t ret = sequencer_publish(fp, blck);
	if(ret == SEQ_SUCCESS)
	{
		batch.push_back(blck);
		print_block(blck);
	}
	return ret;
}

static int j = 0;

static void push_ond_da(vector<block*>& batch)
{
	static int mined;
	static int L2_finalized;

	block *blck_batch = new block;
	block_init(blck_batch);

	for(auto& block : batch)
	{
		block_copy(blck_batch, block);
		block->status = DaNotFinalized;
	}

	blck_batch->status = DaNotFinalized;

	L1.push_back(blck_batch);

	if(++j == MINER_COUNT)
	{
		L1[mined]->status = DaFinalized;
		mined++;
		j = 0;
		for(int k = 0; k<BATCH_LENGTH; ++k)
		{
			L2[L2_finalized]->status = DaFinalized;
			L2_finalized++;
		}

	}
}

static void help()
{
	cout << "Welcome to Chainway Senior Engineering Challenge!!" << endl;
	cout << "'p' -> publish a block" << endl;
	cout << "'b' -> publish a batch" << endl;
	cout << "'g' -> Returns a value and state of the given key, usage: g <key>" << endl;
	cout << "'h' -> Returns the value at block height for the given key, usage: h <key> <block>" << endl;
	cout << "'hall' -> Returns all history for the given key, usage: hall <key>" << endl;
	cout << "1 -> REORG 1" << endl;
	cout << "2 -> REORG 2" << endl;
	cout << "3 -> REORG 3" << endl;
	cout << "else, help" << endl;

}

static cmd_t cmd(uint64_t& param1, uint64_t& param2)
{
	string command;
	while(1)
	{
		cin >> command;

		if(command == "p")
			return CMD_PUB;

		if(command == "b")
			return CMD_BATCH;

		if(command == "1")
		{
			param1 = 1;
			return CMD_REORG;
		}
		if(command == "2")
		{
			param1 = 2;
			return CMD_REORG;
		}
		if(command == "3")
		{
			param1 = 3;
			return CMD_REORG;
		}

		if(command == "g")
		{
			cin >> param1;
			return CMD_GET;
		}

		if(command == "h")
		{
			cin >> param1;
			cin >> param2;
			return CMD_HISTORY;
		}

		if(command == "hall")
		{
			cin >> param1;
			return CMD_HISTORY_ALL;
		}

		help();
	}
}

static bool search(vector<block*> blocks, uint64_t key, bool stop_at_first, bool is_batch)
{
	auto it = blocks.rbegin();
	auto const end = blocks.rend();
	auto const begin = it;

	while(it != end)
	{
		if((*it)->mt.contains(key))
		{
			cout << (is_batch ? "Block in the Batch Height: " : "Block Height: ") << -distance(it, begin) << " Value: " << (*it)->data[key] << endl;
			cout << "State: " << state2str((*it)->status) << endl;
			if(stop_at_first)
				break;
		}

		++it;
	}

	return it != end;
}


int main()
{
	FILE* fp;
	seq_ret_t ret = sequencer_init("from_sequencer.txt", &fp);
	int i=0;
	vector<block*> batch;
	uint64_t par1, par2;
	cmd_t comm;

	if(ret != SEQ_SUCCESS)
	{
		cout << "init failed!!" << endl;
		return -1;
	}

	help();

	while(1)
	{

		comm = cmd(par1, par2);

		switch (comm) {
			case CMD_PUB:
			{
				ret = publish_block(fp, batch);
				++i;
				if(ret == SEQ_SUCCESS)
					cout << "A block published!!" << endl;
				else
					cout << "The publishing block failed!!" << endl;
				if(ret == SEQ_SUCCESS && i == BATCH_LENGTH)
				{
					L2.insert(L2.end(), batch.begin(), batch.end());
					push_ond_da(batch);
					batch.clear();
					i = 0;
					cout << "A batch published on DA!!" << endl;
				}
				break;
			}
			case CMD_BATCH:
			{
				while(i<BATCH_LENGTH && ret == SEQ_SUCCESS)
				{
					ret = publish_block(fp, batch);
					++i;
				}
				if(ret == SEQ_SUCCESS)
				{
					L2.insert(L2.end(), batch.begin(), batch.end());
					push_ond_da(batch);
					batch.clear();
					i = 0;
					cout << "A batch published on DA!!" << endl;
				}
				else
				{
					cout << "The batch publishing on DA Failed!!" << endl;
				}
				break;
			}

			case CMD_GET:
			{
				cout << "Looking for key: " << par1 << endl;
				bool found = search(batch, par1, true, true);
				if(!found)
					found = search(L1, par1, true, false);
				if(!found)
					cout << "Key: " << par1 << " not Present!" << endl;
				break;
			}
			case CMD_HISTORY_ALL:
			{
				cout << "Viewing the entire history for key: " << par1 << endl;
				search(batch, par1, false, true);
				search(L2, par1, false, false);
				break;
			}
			case CMD_REORG:
			{
				while(par1 > 0 && j > 0)
				{
					L1.pop_back();
					j--;
					par1--;
					cout << "REORG!!" << endl;

				}
				if(par1)
					cout << "REORG Failed: DA is empty or all blocks have been confirmed!" << endl;
				break;
			}

			case CMD_HISTORY:
			{
				cout << "Looking for key: " << par1 << " Block Height: " << par2 << endl;

				auto idx = L2.size() - par2 - 1;
				if(idx >= 0)
				{
					if(L2[idx]->mt.contains(par1))
						cout << "Val: " << L2[idx]->data[par1] << endl;
					else
						cout << "Key " << par1 << " not present!" << endl;
				}
				else
				{
					cout << "Invalid Block Height " << par2 << endl;
				}

				break;
			}

			default:
				break;
		}

	}

	return sequencer_close(fp);
}
