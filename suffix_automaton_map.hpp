template <class T>
class SuffixAutomaton
{
public:
	class Node
	{
	public:
		int len;
		int slnk;
		map<T, int> son;

		Node(void) : len(0), slnk(-1)
		{

		}
	};

	int sz, cur;
	vector<Node> nodes;

	SuffixAutomaton(void)
	{
		clear();
	}

	Node & operator [] (const int & x)
	{
		return nodes[x];
	}

	void clear(void)
	{
		sz = cur = 0;
		nodes.clear();
		add_node();
	}

	int add_node(void)
	{
		nodes.push_back(Node());
		return sz++;
	}

	Node clone(const Node & node)
	{
		Node ans;
		ans.len = node.len;
		ans.slnk = node.slnk;
		ans.son = node.son;
		return ans;
	}

	int extend(const T & c)
	{
		int last = cur;
		bool flg = nodes[last].count(c);
		if (!flg)
		{
			cur = add_node();
			nodes[cur].len = nodes[last].len + 1;
		}
		else
			cur = nodes[last].son[c];
		int p = last;
		for (; p != -1 && !nodes[p].son.count(c); p = nodes[p].slnk)
			nodes[p].son[c] = cur;
		if (p == -1)
			nodes[cur].slnk = 0;
		else
		{
			int q = nodes[p].son[c];
			if (nodes[p].len + 1 == nodes[q].len)
			{
				if (!flg)
					nodes[cur].slnk = q;
			}
			else
			{
				int r = add_node();
				nodes[r] = clone(nodes[q]);
				nodes[r].len = nodes[p].len + 1;
				nodes[q].slnk = nodes[cur].slnk = r;
				for (; p != -1 && nodes[p].son.count(c) && nodes[p].son[c] == q; p = nodes[p].slnk)
					nodes[p].son[c] = r;
				if (flg)
					cur = r;
			}
		}
		return cur;
	}

	void build(const T * first, const T * last)
	{
		const T * & s = first;
		const int n = last - first;

		clear();
		for (int i = 0; i < n; i++)
			extend(s[i]);
	}

	vector<int> sorted_indices(void)
	{
		int mxlen = 0;
		for (int u = 0; u < sz; u++)
			mxlen = max(mxlen, nodes[u].len);
		vector<int> occ(mxlen + 1);
		for (int u = 0; u < sz; u++)
			occ[nodes[u].len]++;
		for (int i = 1; i <= mxlen; i++)
			occ[i] += occ[i - 1];
		vector<int> ans(sz);
		for (int u = 0; u < sz; u++)
			ans[--occ[nodes[u].len]] = u;
		return ans;
	}

	vector<int> sorted_indices(const vector<int> & qry)
	{
		int mxlen = 0;
		for (const auto & u : qry)
			mxlen = max(mxlen, nodes[u].len);
		vector<int> occ(mxlen + 1);
		for (const auto & u : qry)
			occ[nodes[u].len]++;
		for (int i = 1; i <= mxlen; i++)
			occ[i] += occ[i - 1];
		vector<int> ans(qry.size());
		for (const auto & u : qry)
			ans[--occ[nodes[u].len]] = u;
		return ans;
	}
};

