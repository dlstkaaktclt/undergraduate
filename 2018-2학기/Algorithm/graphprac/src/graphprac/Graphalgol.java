package graphprac;
import java.util.ArrayList;
import java.util.Queue;

public class Graphalgol {
	
	private int vertex[];
	private AdjacentList adj[];
	
	
	
	class AdjacentList
	{
		private ArrayList<Adj> adjlist;
		
		public AdjacentList()
		{
			this.adjlist = new ArrayList<Adj>();
		}
		
		public void addAdjList(int x,int y,int z)
		{
			Adj temp = new Adj(x,y,z);
			this.adjlist.add(temp);
		}
		
		public ArrayList<Adj> getAdjList()
		{
			return this.adjlist;
		}
		
	}
	
	class Adj
	{
		private int x,y,z;
		
		public Adj(int x,int y,int z)
		{
			this.x = x;
			this.y = y;
			this.z = z;
		}
		
		public void setfst(int x)
		{
			this.x = x;
		}
		public int getfst()
		{
			return this.x;
		}
		
		public void setsnd(int y)
		{
			this.y = y;
		}
		public int getsnd()
		{
			return this.y;
		}
		
		public void settrd(int z)
		{
			this.z = z;
		}
		public int gettrd()
		{
			return this.z;
		}
		
	}
	
    class tuple3<X,Y,Z>
	{
		private X x;
		private Y y;
		private Z z;
		
		public tuple3(X x,Y y,Z z)
		{
			this.x = x;
			this.y = y;
			this.z = z;
		}
		
		public void setfst(X x)
		{
			this.x = x;
		}
		public X getfst()
		{
			return this.x;
		}
		
		public void setsnd(Y y)
		{
			this.y = y;
		}
		public Y getsnd()
		{
			return this.y;
		}
		
		public void settrd(Z z)
		{
			this.z = z;
		}
		public Z gettrd()
		{
			return this.z;
		}
		
	}
	
	public class DisjointSet
	{
		int num;
		DisjointSet parent;
		int rank;
		
		public DisjointSet(int num)
		{
			this.num = num;
			parent = this;
			this.rank = 0;
		}
		
		public DisjointSet find(DisjointSet other)
		{
			if(other.parent == other)
			{
				return other;
			}
			else
			{
				DisjointSet root = find(other.parent);
				other.setParent(root);
				return root;
			}
		}
		
		public void union(DisjointSet one,DisjointSet two)
		{
			DisjointSet fst = find(one);
			DisjointSet snd = find(two);
			
			if(fst.getNum() != snd.getNum())
			{
				int fstrank = fst.getRank();
				int sndrank = snd.getRank();
				
				if(fstrank>sndrank)
				{
					snd.setParent(fst);
				}
				else if(fstrank<sndrank)
				{
					fst.setParent(snd);
				}
				else
				{
					snd.setParent(fst);
					fst.rankUp();
				}
			}
		}
		
		public int getNum()
		{
			return this.num;
		}
		
		public void setParent(DisjointSet p)
		{
			this.parent = p;
		}
		
		public int getRank()
		{
			return this.rank;
		}
		
		public void rankUp()
		{
			this.rank++;
		}
		
		public boolean isSameset(DisjointSet one,DisjointSet two)
		{
			DisjointSet fst = find(one);
			DisjointSet snd = find(two);
			
			if(fst.getNum()==snd.getNum()) return true;
			else return false;
		}
		
	}
    
    public class fineHeap()
    {
    	
    	
    }
	
	
	
	
	
    public void adfs()
    {
    	for(int i=1;i<=vertexnum;i++)
    	{
    		visited[i] = false;
    	}
    	
    	for(int i=1;i<=vertexnum;i++)
    	{
    		if(!visited[i])
    		{
    			dfs(vertex[i]);
    		}
    	}
    }
    
	public void dfs(v)
	{
		visited[v]=true;
		for(int i=1;i<=length(v.adj);i++)
		{
			if(!visited[v.adj[i].getnode()])
			{
				dfs(v.adj[i].getnode());
			}
		}
		
		
	}
	
	public void bfs()
	{
		for (int i=1;i<=vertexnum;i++)
		{
			visited[i] = false;
		}
		visited[s] = true;
		
		Queue<E> queue;
		queue.enqueue(vertex[s]);
		while (!queue.isEmpty()) {
			vertex v = queue.dequeue();
			for(int i=1;i<=length(v.adj);i++)
			{
				if(!visited[v.adj[i].getnode()])
				{
					visited[v.adj[i].getnode()] = true;
					queue.enqueue(v.adj[i].getnode());
				}
			}
		}	
	}
	
	public int prim(Graph G)
	{
		
	}
	
	
	
	public static void main(String args[])
	{
		
	}
	
	
	
	
	

}
