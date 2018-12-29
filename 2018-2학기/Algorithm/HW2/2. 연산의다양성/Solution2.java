import java.util.StringTokenizer;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.PrintWriter;

/*
   1. 아래와 같은 명령어를 입력하면 컴파일이 이루어져야 하며, Solution4 라는 이름의 클래스가 생성되어야 채점이 이루어집니다.
       javac Solution2.java -encoding UTF8


   2. 컴파일 후 아래와 같은 명령어를 입력했을 때 여러분의 프로그램이 정상적으로 출력파일 output4.txt 를 생성시켜야 채점이 이루어집니다.
       java Solution2

   - 제출하시는 소스코드의 인코딩이 UTF8 이어야 함에 유의 바랍니다.
   - 수행시간 측정을 위해 다음과 같이 time 명령어를 사용할 수 있습니다.
       time java Solution2
   - 일정 시간 초과시 프로그램을 강제 종료 시키기 위해 다음과 같이 timeout 명령어를 사용할 수 있습니다.
       timeout 0.5 java Solution2   // 0.5초 수행
       timeout 1 java Solution2     // 1초 수행
 */

class Solution2 {

	/*
		** 주의사항
		정답의 숫자가 매우 크기 때문에 답안은 반드시 int 대신 long 타입을 사용합니다.
		그렇지 않으면 overflow가 발생해서 0점 처리가 됩니다.
		Answer[0]을 a의 개수, Answer[1]을 b의 개수, Answer[2]를 c의 개수라고 가정했습니다.
	*/
    static int n;                           // 문자열 길이
    static String s;                        // 문자열
	static long[] Answer = new long[3];     // 정답

	public static void main(String[] args) throws Exception {
		/*
		   동일 폴더 내의 input2.txt 로부터 데이터를 읽어옵니다.
		   또한 동일 폴더 내의 output2.txt 로 정답을 출력합니다.
		 */
		BufferedReader br = new BufferedReader(new FileReader("input2.txt"));
		StringTokenizer stk;
		PrintWriter pw = new PrintWriter("output2.txt");

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
			char[] arr = s.toCharArray();
			
			long[][] A = new long[n+1][n+1];			//A[i][j]는 i~j번째 자리까지 괄호를 쳤을때 결과값으로 a가 나올수 있는 연산의 방법 수
			long[][] B = new long[n+1][n+1];			//B[i][j]는 i~j번째 자리까지 괄호를 쳤을때 결과값으로 b가 나올수 있는 연산의 방법 수
			long[][] C = new long[n+1][n+1];			//C[i][j]는 i~j번째 자리까지 괄호를 쳤을때 결과값으로 c가 나올수 있는 연산의 방법 수
			
			for(int i=1;i<=n;i++)
			{
				if(arr[i-1]=='a') A[i][i] = 1;
				if(arr[i-1]=='b') B[i][i] = 1;
				if(arr[i-1]=='c') C[i][i] = 1;
			}	// 이니셜라이즈. i~i자리까지, 즉 연산을 안하는 경우에는 각 케이스마다 1가지 방법들 존재 -> theta(n) 시간.
			
			
			for(int k=1;k<=n-1;k++)	// k는 문제 스케일. scale = k+1
			{
				for(int i=1;i<=n-k;i++)
				{
					long Aval=0;
					long Bval=0;
					long Cval=0;
					for(int j=i;j<=i+k-1;j++)		
					{
						Aval+=C[i][j]*A[j+1][i+k];
						Aval+=A[i][j]*C[j+1][i+k];
						Aval+=B[i][j]*C[j+1][i+k];
					}
					for(int j=i;j<=i+k-1;j++)
					{
						Bval+=A[i][j]*B[j+1][i+k];
						Bval+=B[i][j]*B[j+1][i+k];
						Bval+=A[i][j]*A[j+1][i+k];
					}
					for(int j=i;j<=i+k-1;j++)
					{
						Cval+=C[i][j]*B[j+1][i+k];
						Cval+=B[i][j]*A[j+1][i+k];
						Cval+=C[i][j]*C[j+1][i+k];
					}
					A[i][i+k] = Aval;
					B[i][i+k] = Bval;
					C[i][i+k] = Cval;			//각 abc마다 그 아래의 모든 케이스들의 결과를 통해 i~i+k번째 까지 자리의 값을 구한다.
				}								//k = 1 to n-1, i = 1 to n-k, j = i to i+k 에서 총 소요시간은 theta(n^3)의 시간이 걸림.
			}	
			/////////////////////////////////////////////////////////////////////////////////////////////
			/*
			   이 부분에서 여러분의 알고리즘이 수행됩니다.
			   정답을 구하기 위해 주어진 문자열 s를 여러분이 원하시는 대로 가공하셔도 좋습니다.
			   문제의 답을 계산하여 그 값을 Answer(long 타입!!)에 저장하는 것을 가정하였습니다.
			 */
			//////////////////////////////////////////////////////////////////////////////////////////////
						
			//따라서 이 알고리즘의 전체 시간복잡도는 theta(n^3)이다.ㄴ
			Answer[0] = A[1][n];  // a 의 갯수 . 1~n자리까지 연산했을때 나올수 있는 A의 가짓수
			Answer[1] = B[1][n];  // b 의 갯수. 1~n자리까지 연산했을때 나올수 있는 B의 가짓수
			Answer[2] = C[1][n];  // c 의 갯수. 1~n자리까지 연산했을때 나올수 있는 C의 가짓수


			// output4.txt로 답안을 출력합니다.
			pw.println("#" + test_case + " " + Answer[0] + " " + Answer[1] + " " + Answer[2]);
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

