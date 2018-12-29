import java.util.StringTokenizer;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.PrintWriter;

/*
   1. 아래와 같은 명령어를 입력하면 컴파일이 이루어져야 하며, Solution4 라는 이름의 클래스가 생성되어야 채점이 이루어집니다.
       javac Solution3.java -encoding UTF8


   2. 컴파일 후 아래와 같은 명령어를 입력했을 때 여러분의 프로그램이 정상적으로 출력파일 output4.txt 를 생성시켜야 채점이 이루어집니다.
       java Solution3

   - 제출하시는 소스코드의 인코딩이 UTF8 이어야 함에 유의 바랍니다.
   - 수행시간 측정을 위해 다음과 같이 time 명령어를 사용할 수 있습니다.
       time java Solution3
   - 일정 시간 초과시 프로그램을 강제 종료 시키기 위해 다음과 같이 timeout 명령어를 사용할 수 있습니다.
       timeout 0.5 java Solution3   // 0.5초 수행
       timeout 1 java Solution3     // 1초 수행
 */

class Solution3 {
	static final int max_n = 100000;

	static int n;
	static String s;
	static int Answer;
	
	public static void main(String[] args) throws Exception {
		/*
		   동일 폴더 내의 input3.txt 로부터 데이터를 읽어옵니다.
		   또한 동일 폴더 내의 output3.txt 로 정답을 출력합니다.
		 */
		BufferedReader br = new BufferedReader(new FileReader("input3.txt"));
		StringTokenizer stk;
		PrintWriter pw = new PrintWriter("output3.txt");

		/*
		   10개의 테스트 케이스가 주어지므로, 각각을 처리합니다.
		 */
		for (int test_case = 1; test_case <= 10; test_case++) {
			/*
			   각 테스트 케이스를 파일에서 읽어옵니다.
               첫 번째 행에 쓰여진 문자열의 길이를 n에 읽어들입니다.
               그 다음 행에 쓰여진 문자열을 s에 한번에 읽어들입니다.
			 */
			stk = new StringTokenizer(br.readLine());
			n = Integer.parseInt(stk.nextToken());
			s = br.readLine();
			StringBuffer rev = new StringBuffer(s); // to save reverse string
			char[] arr = s.toCharArray();		//문자열 저장한 어레이
			char[] revarr = rev.reverse().toString().toCharArray();		//문자열을 뒤집은 문자열을 저장한 어레이. 
			//문자열과 그 문자열을 reverse한 문자열의 최대 부분문자열이 우리가 구하고자 하는 답이다.
			
			int start=0;
			int revstart=0;
			int end = arr.length;
			int revend = revarr.length;
			
			int[][] maxarr = new int[end+1][revend+1];		// maxarr[i][j]는  문자열의 i번째문자까지와 뒤집은 문자열의 j번째문자까지의 최대 부분문자열의 길이를 담는다.
			for(int i=1;i<=end;i++)
			{
				for(int j=1;j<=revend;j++)
				{
					if(arr[i-1]==revarr[j-1])
					{
						maxarr[i][j] = maxarr[i-1][j-1] + 1;		// i번째와 j번째 문자가 같은경우
					}
					else
					{
						maxarr[i][j] = Math.max(maxarr[i-1][j],maxarr[i][j-1]);	//i번째와 j번째 문자가 다른경우
					}
				}
			}
			// i = 1 to 문자열 길이(n) , j 1 to 문자열 길이(n)만큼 실행되므로 총 theta(n^2)의 수행시간을 가진다.
			/////////////////////////////////////////////////////////////////////////////////////////////
			/*
			   이 부분에서 여러분의 알고리즘이 수행됩니다.
			   문제의 답을 계산하여 그 값을 Answer에 저장하는 것을 가정하였습니다.
			 */
			/////////////////////////////////////////////////////////////////////////////////////////////
			Answer = maxarr[end][revend];
			//따라서 이 알고리즘의 전체 시간복잡도는 theta(n^2)이다.

			// output3.txt로 답안을 출력합니다.
			pw.println("#" + test_case + " " + Answer);
			/*
			   아래 코드를 수행하지 않으면 여러분의 프로그램이 제한 시간 초과로 강제 종료 되었을 때,
			   출력한 내용이 실제로 파일에 기록되지 않을 수 있습니다.
			   따라서 안전을 위해 반드시 flush() 를 수행하시기 바랍니다.
			 */
			pw.flush();
		}

		br.close();
		pw.close();
	}
	
	
	
}

