#include "smt.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include "sequencer.h"
#include "block.h"

using namespace std;

#define PUBLISH_COUNT	5
#define MINER_COUNT		3



static seq_ret_t publish_block(FILE* fp, vector<block*>& batch)
{
	block *blck = new block;
	block_init(blck);
	seq_ret_t ret = sequencer_publish(fp, blck);
	if(ret == SEQ_SUCCESS)
		batch.push_back(blck);
	return ret;
}



static void push_ond_da(vector<block*>& L1, block *block)
{
	static int j, mined;
	L1.push_back(block);

	if(j++ == MINER_COUNT)
	{
		L1[mined]->status = DaFinalized;
		mined++;
		j = 0;
	}
}

int main()
{
	FILE* fp;
	vector<block*> L2;
	vector<block*> L1;

	seq_ret_t ret = sequencer_init("from_sequencer.txt", &fp);
	if(ret != SEQ_SUCCESS)
	{
		cout << "init failed!!" << endl;
		return -1;
	}
	int i=0;

	int cmd;

	while(ret == SEQ_SUCCESS)
	{
		vector<block*> batch;
		block *blck_batch = new block;
		block_init(blck_batch);
		while(i<PUBLISH_COUNT && ret == SEQ_SUCCESS)
		{
			ret = publish_block(fp, batch);
			if(ret == SEQ_SUCCESS)
			{
				block_copy(blck_batch, batch.back());
			}

			++i;
		}

		L2.insert(L2.end(), batch.begin(), batch.end());
		batch.clear();
		push_ond_da(L1, blck_batch);

		i = 0;
	}

	uint64_t key = 32;


	auto it = L1.rbegin();
	const auto begin = it;
	auto end = L1.rend();

	while(it != end)
	{
		if((*it)->mt.contains(key))
		{
			cout << "block: " << std::distance(begin, it) << " val: " << (*it)->data[key] << endl;
		}
		++it;
	}



	return sequencer_close(fp);

}
