package moonalgol;
import java.util.*;



public class Sorting {
	
	final static int arraySize=25;
	
	
	@FunctionalInterface
	interface sortFunc {
		public void sort(int[] arr, int n);
	}
	
	public static int[] makeRandomIntArray(int n)
	{
		int[] arr = new int[n];
		Random r = new Random();
		int randnum;
		
		for(int i=0;i<n;i++)
		{
			arr[i] = i+1;
		}
		
		for(int i=0;i<n;i++)
		{
			randnum = r.nextInt(n);
			int temp;
			temp = arr[i];
			arr[i] = arr[randnum];
			arr[randnum] = temp;
		}
		
		return arr;
		
	}
	
	public static void printArray(int[] array)
	{
		for(int i=0;i<array.length;i++)
		{
			System.out.print(array[i] + " ");
		}
		System.out.println();
	}
	
	
	public static void main(String args[])
	{
		sorting((arr,n) -> selectionSort(arr,n));
		sorting((arr,n) -> bubbleSort(arr,n));
		sorting((arr,n) -> recursiveSelectionSort(arr,n));
		sorting((arr,n) -> insertionSort(arr,n));
		sorting((arr,n) -> mergeWrap(arr,n));
		sorting((arr,n) -> quickWrap(arr,n));
		sorting((arr,n) -> heapSort(arr,n));
	}
	
	public static void sorting(sortFunc func)
	{
		int arr[] = makeRandomIntArray(arraySize);
		printArray(arr);
		
		func.sort(arr,arr.length);
		printArray(arr);
	}
	
	public static void selectionSort(int arr[],int n)
	{
		int temp,index;
		for(int i=n-1;i>0;i--)	// for i -> n to 2, do sorting
		{
			int maximum=arr[i];			
			index = i;
			for(int j=i;j>=0;j--)
			{
				if(arr[j]>maximum)
				{
					maximum = arr[j];
					index = j;
				}
			} // find maximum for j -> i to 1
			temp = arr[i];
			arr[i] = arr[index];
			arr[index] = temp; // swap maximum & i-th member
		}
	}
	
	public static int findMaxindex(int arr[],int n)
	{
		int maxindex;
		if(n==1) return 0;
		else return (arr[n-1] > (arr[maxindex=findMaxindex(arr,n-1)])) ? n-1 : maxindex;
	}
	
	public static void swap(int arr[],int a,int b)
	{
		int temp;
		temp = arr[a];
		arr[a] = arr[b];
		arr[b] = temp;
	}
	
	public static void recursiveSelectionSort(int arr[],int n)
	{
		if(n == 1) return;
		int index = findMaxindex(arr,n);
		swap(arr,n-1,index);
		recursiveSelectionSort(arr,n-1);
	}
	
	
	public static void bubbleSort(int arr[],int n)
	{
		int temp;
		for(int i=n-1;n>0;n--)	// for i -> n to 2, do sorting
		{
			for(int j=0;j<i;j++)	// for j -> 1 to i-1th, do sorting
			{
				if(arr[j]>arr[j+1])	// if nextmember is smaller than now one, swap()
				{
					temp = arr[j];
					arr[j] = arr[j+1];
					arr[j+1] = temp;
				}
			}
		}
	}
	
	public static void insertionSort(int arr[],int n)
	{
		int temp;
		boolean sorted;
		for(int i=0;i<n;i++)
		{
			sorted=false;
			temp = arr[i];
			for(int j=i-1;j>=0;j--)
			{
				if(arr[j]>temp)
				{
					arr[j+1] = arr[j];
				}
				else
				{
					arr[j+1] = temp;
					sorted=true;
					break;
				}
			}
			if(!sorted) arr[0] = temp;
		}
	}
	
	public static void mergeWrap(int arr[],int n)
	{
		mergeSort(arr,0,n-1);
	}
	
	public static void mergeSort(int arr[],int h,int t)
	{
		if(h<t)
		{
			int p = (h+t)/2;
			mergeSort(arr,h,p);
			mergeSort(arr,p+1,t);
			merge(arr,h,p,t);
		}
	}
	
	public static void merge(int arr[],int h,int p,int t)	//merge가 개 복잡함..
	{
		int low=h,high=p+1;
		int i=0;
		int arr2[] = new int[(t-h+1)];
		while(low!=(p+1)&&high!=(t+1))
		{
			if(arr[low] < arr[high])
			{
				arr2[i]=arr[low];
				i++;
				low++;
			}
			else
			{
				arr2[i]=arr[high];
				i++;
				high++;
			}
		}
		if(low==p+1)
		{
			while(high!=(t+1))
			{
				arr2[i]=arr[high];
				i++;
				high++;
			}
		}
		if(high==t+1)
		{
			while(low!=(p+1))
			{
				arr2[i]=arr[low];
				i++;
				low++;
			}
		}
		for(int k=0;k<t-h+1;k++)
		{
			arr[k+h] = arr2[k];
		}
	}
	
	public static void quickWrap(int arr[],int n)
	{
		quickSort(arr,0,n-1);
	}
	
	public static void quickSort(int arr[],int a,int b)
	{
		if(a<b)
		{
			int r = partition(arr,a,b);
			quickSort(arr,a,r-1);
			quickSort(arr,r+1,b);
		}
		
	}
	
	public static int partition(int arr[],int a,int b)
	{
		int temp;
		int pivot=arr[b];
		int lowend=a-1,next=a;
		while(next!=b)
		{
			if(arr[next]<pivot)
			{
				swap(arr,next,lowend+1);
				next++;
				lowend++;
			}
			else
			{
				next++;
			}
		}
		swap(arr,b,lowend+1);
		return lowend+1;
	}
	
	public static int extractMin(int arr[],int heapsize)
	{
		int minimum = arr[0];
		if(heapsize>=1)
		{
			int temp = arr[0];
			arr[0]=arr[heapsize-1];
			arr[heapsize-1] = temp;
			percolateDownMinheap(0,arr,heapsize-1);
		}
		return minimum;
	}
	
	public static void percolateDownMinheap(int now,int arr[],int n) //version index 0,1,2....n-1.   n means heapsize.
	{
		int leftchild = 2*now + 1;
		int rightchild = 2*now + 2;
		
		if(leftchild>(n-1))
		{
			return;
		}
		else if(leftchild==(n-1))
		{
			if(arr[leftchild]<arr[now])
			{
				int temp = arr[now];
				arr[now] = arr[leftchild];
				arr[leftchild] = arr[now];
			}
		}
		else
		{
			int smaller = arr[leftchild]<arr[rightchild] ? leftchild : rightchild;
			if(arr[smaller]<arr[now])
			{
				int temp = arr[now];
				arr[now] = arr[smaller];
				arr[smaller] = temp;
				percolateDownMinheap(smaller,arr,n);
			}
		}
	}
	
	public static void buildMinheap(int arr[],int n)
	{
		for(int i=(n/2);i>=0;i--)
		{
			percolateDownMinheap(i,arr,n);
		}
	}
	
	
	public static void heapSort(int arr[],int n)
	{
		buildMinheap(arr,n);
		
		for(int i=n;i>0;i--)
		{
			int temp = extractMin(arr,i);
		}
		
	}
	
	
	
	
	
	
	/*
	public static void recursiveBuildMaxHeap(int arr[],int h,int n)
	{
		if(h>=n) return;
		else
		{
			int now=h+1;
			int next;
			
			while((now!=0) && (next=((now-1)/2))
			arr[ ];
			recursiveBuildHeap(arr,h+1,n);
		}
	}
	*/
	/*
	public static void buildMaxHeap(int arr[],int n)
	{
		recursiveBuildMaxHeap(arr,0,n-1);
	}
	
	public static void heapSort(int arr[],int n)
	{
		buildMaxHeap(arr,n);
		
	}
	*/
	/*
	public static void recursiveInsertionSort(int arr[],int n)
	{
		recursiveInsertion(arr,0,n-1); // sorted index and end index
	}
	
	public static void recursiveInsertion(int arr[],int s,int f)
	{
		if(s==f) return;
		else
		{
			index = findLoca(arr,s,member);
			shift(arr,index,s+1);
			insert(arr,index,member);
			recursiveInsertion(arr,s+1,f);
		}
	}
	
	*/
}
