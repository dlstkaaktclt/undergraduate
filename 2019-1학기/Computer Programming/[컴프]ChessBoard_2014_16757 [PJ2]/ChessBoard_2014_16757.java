package chess;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;
import javax.swing.*;
import javax.swing.border.*;
import java.math.*;

//======================================================Don't modify below===============================================================//
enum PieceType {king, queen, bishop, knight, rook, pawn, none}
enum PlayerColor {black, white, none}

public class ChessBoard_2014_16757 {
    private final JPanel gui = new JPanel(new BorderLayout(3, 3));
    private JPanel chessBoard;
    private JButton[][] chessBoardSquares = new JButton[8][8];
    private Piece[][] chessBoardStatus = new Piece[8][8];
    private ImageIcon[] pieceImage_b = new ImageIcon[7];
    private ImageIcon[] pieceImage_w = new ImageIcon[7];
    private JLabel message = new JLabel("Enter Reset to Start");

    ChessBoard_2014_16757(){
        initPieceImages();
        initBoardStatus();
        initializeGui();
    }

    public final void initBoardStatus(){
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++) chessBoardStatus[j][i] = new Piece(i, j);  // modified for piece.
        }
    }

    public final void initPieceImages(){
        pieceImage_b[0] = new ImageIcon(new ImageIcon("./img/king_b.png").getImage().getScaledInstance(64, 64, java.awt.Image.SCALE_SMOOTH));
        pieceImage_b[1] = new ImageIcon(new ImageIcon("./img/queen_b.png").getImage().getScaledInstance(64, 64, java.awt.Image.SCALE_SMOOTH));
        pieceImage_b[2] = new ImageIcon(new ImageIcon("./img/bishop_b.png").getImage().getScaledInstance(64, 64, java.awt.Image.SCALE_SMOOTH));
        pieceImage_b[3] = new ImageIcon(new ImageIcon("./img/knight_b.png").getImage().getScaledInstance(64, 64, java.awt.Image.SCALE_SMOOTH));
        pieceImage_b[4] = new ImageIcon(new ImageIcon("./img/rook_b.png").getImage().getScaledInstance(64, 64, java.awt.Image.SCALE_SMOOTH));
        pieceImage_b[5] = new ImageIcon(new ImageIcon("./img/pawn_b.png").getImage().getScaledInstance(64, 64, java.awt.Image.SCALE_SMOOTH));
        pieceImage_b[6] = new ImageIcon(new BufferedImage(64, 64, BufferedImage.TYPE_INT_ARGB));

        pieceImage_w[0] = new ImageIcon(new ImageIcon("./img/king_w.png").getImage().getScaledInstance(64, 64, java.awt.Image.SCALE_SMOOTH));
        pieceImage_w[1] = new ImageIcon(new ImageIcon("./img/queen_w.png").getImage().getScaledInstance(64, 64, java.awt.Image.SCALE_SMOOTH));
        pieceImage_w[2] = new ImageIcon(new ImageIcon("./img/bishop_w.png").getImage().getScaledInstance(64, 64, java.awt.Image.SCALE_SMOOTH));
        pieceImage_w[3] = new ImageIcon(new ImageIcon("./img/knight_w.png").getImage().getScaledInstance(64, 64, java.awt.Image.SCALE_SMOOTH));
        pieceImage_w[4] = new ImageIcon(new ImageIcon("./img/rook_w.png").getImage().getScaledInstance(64, 64, java.awt.Image.SCALE_SMOOTH));
        pieceImage_w[5] = new ImageIcon(new ImageIcon("./img/pawn_w.png").getImage().getScaledInstance(64, 64, java.awt.Image.SCALE_SMOOTH));
        pieceImage_w[6] = new ImageIcon(new BufferedImage(64, 64, BufferedImage.TYPE_INT_ARGB));
    }

    public ImageIcon getImageIcon(Piece piece){
        if(piece.color.equals(PlayerColor.black)){
            if(piece.type.equals(PieceType.king)) return pieceImage_b[0];
            else if(piece.type.equals(PieceType.queen)) return pieceImage_b[1];
            else if(piece.type.equals(PieceType.bishop)) return pieceImage_b[2];
            else if(piece.type.equals(PieceType.knight)) return pieceImage_b[3];
            else if(piece.type.equals(PieceType.rook)) return pieceImage_b[4];
            else if(piece.type.equals(PieceType.pawn)) return pieceImage_b[5];
            else return pieceImage_b[6];
        }
        else if(piece.color.equals(PlayerColor.white)){
            if(piece.type.equals(PieceType.king)) return pieceImage_w[0];
            else if(piece.type.equals(PieceType.queen)) return pieceImage_w[1];
            else if(piece.type.equals(PieceType.bishop)) return pieceImage_w[2];
            else if(piece.type.equals(PieceType.knight)) return pieceImage_w[3];
            else if(piece.type.equals(PieceType.rook)) return pieceImage_w[4];
            else if(piece.type.equals(PieceType.pawn)) return pieceImage_w[5];
            else return pieceImage_w[6];
        }
        else return pieceImage_w[6];
    }

    public final void initializeGui(){
        gui.setBorder(new EmptyBorder(5, 5, 5, 5));
        JToolBar tools = new JToolBar();
        tools.setFloatable(false);
        gui.add(tools, BorderLayout.PAGE_START);
        JButton startButton = new JButton("Reset");
        startButton.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){
                initiateBoard();
            }
        });

        tools.add(startButton);
        tools.addSeparator();
        tools.add(message);

        chessBoard = new JPanel(new GridLayout(0, 8));
        chessBoard.setBorder(new LineBorder(Color.BLACK));
        gui.add(chessBoard);
        ImageIcon defaultIcon = new ImageIcon(new BufferedImage(64, 64, BufferedImage.TYPE_INT_ARGB));
        Insets buttonMargin = new Insets(0,0,0,0);
        for(int i=0; i<chessBoardSquares.length; i++) {
            for (int j = 0; j < chessBoardSquares[i].length; j++) {
                JButton b = new JButton();
                b.addActionListener(new ButtonListener(i, j));
                b.setMargin(buttonMargin);
                b.setIcon(defaultIcon);
                if((j % 2 == 1 && i % 2 == 1)|| (j % 2 == 0 && i % 2 == 0)) b.setBackground(Color.WHITE);
                else b.setBackground(Color.gray);
                b.setOpaque(true);
                b.setBorderPainted(false);
                chessBoardSquares[j][i] = b;
            }
        }

        for (int i=0; i < 8; i++) {
            for (int j=0; j < 8; j++) chessBoard.add(chessBoardSquares[j][i]);

        }
    }

    public final JComponent getGui() {
        return gui;
    }

    public static void main(String[] args) {
        Runnable r = new Runnable() {
            @Override
            public void run() {
                ChessBoard_2014_16757 cb = new ChessBoard_2014_16757();
                JFrame f = new JFrame("Chess");
                f.add(cb.getGui());
                f.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                f.setLocationByPlatform(true);
                f.setResizable(false);
                f.pack();
                f.setMinimumSize(f.getSize());
                f.setVisible(true);
            }
        };
        SwingUtilities.invokeLater(r);
    }

    //================================Utilize these functions========================================//

    class Piece{        //modified have more methods and current position.
        PlayerColor color;
        PieceType type;
        int x;                          // this piece's x position
        int y;                          // this piece's y position.
        private boolean marked;         // for check this piece is marked

        Piece(int x, int y){
            color = PlayerColor.none;
            type = PieceType.none;
            setLocation(x, y);
            marked = false;
        }
        Piece(PlayerColor color, PieceType type, int x, int y){
            this.color = color;
            this.type = type;
            setLocation(x, y);
            marked = false;
        }

        public void setLocation(int x, int y)
        {
            if(x < 0) this.x = 0;
            else if(x > 7) this.x = 7;
            else this.x = x;

            if(y < 0) this.y = 0;
            else if(y > 7) this.y = 7;
            else this.y = y;
        }

        public PlayerColor getColor()
        {
            return this.color;
        }

        public PieceType getType()
        {
            return this.type;
        }

        public boolean getMarked()
        {
            return this.marked;
        }

        public int getX()
        {
            return this.x;
        }

        public int getY()
        {
            return this.y;
        }

        public void mark()
        {
            this.marked = true;
        }

        public void unmark()
        {
            this.marked = false;
        }


        // mark tiles this piece could action.
        public void markTiles()
        {}

        // unmark tiles this piece could action.
        public void unmarkTiles()
        {}

        // do action to other piece. (by location)
        public void doActionTo(int x, int y)
        {}

        // return this piece can move to location.
        public boolean canMove(int x, int y)
        {
            return false;
        }

        // return this piece can attact to location.
        public boolean canAttack(int x, int y)
        {
            return false;
        }
    }

    public void setIcon(int x, int y, Piece piece){
        chessBoardSquares[y][x].setIcon(getImageIcon(piece));
        chessBoardStatus[y][x] = piece;
    }

    public Piece getIcon(int x, int y){
        return chessBoardStatus[y][x];
    }

    public void markPosition(int x, int y){
        chessBoardSquares[y][x].setBackground(Color.pink);
    }

    public void unmarkPosition(int x, int y){
        if((y % 2 == 1 && x % 2 == 1)|| (y % 2 == 0 && x % 2 == 0)) chessBoardSquares[y][x].setBackground(Color.WHITE);
        else chessBoardSquares[y][x].setBackground(Color.gray);
    }

    public void setStatus(String inpt){
        message.setText(inpt);
    }

    public void initiateBoard(){        //modified : for make derived class instead of base class Piece.
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++) setIcon(i, j, new EmptyPiece(i, j));           //modified for make derived class empty Piece.
        }
        setIcon(0, 0, new RookPiece(PlayerColor.black, PieceType.rook, 0, 0));
        setIcon(0, 1, new KnightPiece(PlayerColor.black, PieceType.knight, 0, 1));
        setIcon(0, 2, new BishopPiece(PlayerColor.black, PieceType.bishop, 0, 2));
        setIcon(0, 3, new QueenPiece(PlayerColor.black, PieceType.queen, 0, 3));
        setIcon(0, 4, new KingPiece(PlayerColor.black, PieceType.king, 0, 4));
        setIcon(0, 5, new BishopPiece(PlayerColor.black, PieceType.bishop, 0, 5));
        setIcon(0, 6, new KnightPiece(PlayerColor.black, PieceType.knight, 0, 6));
        setIcon(0, 7, new RookPiece(PlayerColor.black, PieceType.rook, 0, 7));
        for(int i=0;i<8;i++){
            setIcon(1, i, new PawnPiece(PlayerColor.black, PieceType.pawn, 1, i));
            setIcon(6, i, new PawnPiece(PlayerColor.white, PieceType.pawn, 6, i));
        }
        setIcon(7, 0, new RookPiece(PlayerColor.white, PieceType.rook, 7, 0));
        setIcon(7, 1, new KnightPiece(PlayerColor.white, PieceType.knight, 7, 1));
        setIcon(7, 2, new BishopPiece(PlayerColor.white, PieceType.bishop, 7, 2));
        setIcon(7, 3, new QueenPiece(PlayerColor.white, PieceType.queen, 7, 3));
        setIcon(7, 4, new KingPiece(PlayerColor.white, PieceType.king, 7, 4));
        setIcon(7, 5, new BishopPiece(PlayerColor.white, PieceType.bishop, 7, 5));
        setIcon(7, 6, new KnightPiece(PlayerColor.white, PieceType.knight, 7, 6));
        setIcon(7, 7, new RookPiece(PlayerColor.white, PieceType.rook, 7, 7));
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++) unmarkPosition(i, j);
        }
        onInitiateBoard();
    }
//======================================================Don't modify above==============================================================//	




    //======================================================Implement below=================================================================//
    enum MagicType {MARK, CHECK, CHECKMATE, NONE};  // not using
    private int selX, selY; // default value is -1, -1. this value save's prev location of selection.
    private boolean check, checkmate, end;  // chessboard flags
    private MagicType chessBoard_currentStatus; // not using.
    private boolean blackturn;      // flag that means this turn is to black.
    private boolean winner_is_black;    // flag that winner is black.

    class ButtonListener implements ActionListener{
        int x;
        int y;
        ButtonListener(int x, int y){
            this.x = x;
            this.y = y;
        }
        public void actionPerformed(ActionEvent e) {	// Only modify here
            // (x, y) is where the click event occured

            Piece prevPiece = getPiece(selX, selY);
            Piece currentPiece = getPiece(this.x, this.y);

            if (end == true) return; // do not anything.
            if (checkmate == true) return; // do not anything.

            if(currentPiece.getMarked()) // if current location is marked, need do action.
            {
                if(prevPiece != null)
                {
                    prevPiece.unmarkTiles();
                    prevPiece.doActionTo(this.x, this.y);
                    set_chessboard_flags();
                    print_chessboard_status();
                }
            }
            else
            {
                if(prevPiece != null)
                {
                    prevPiece.unmarkTiles();
                }
                if(blackturn == true && currentPiece.getColor() == PlayerColor.black)   // if black's turn..
                {
                    currentPiece.markTiles();
                }
                else if(blackturn == false && currentPiece.getColor() == PlayerColor.white) // if white's turn..
                {
                    currentPiece.markTiles();
                }
            }

            selX = this.x;
            selY = this.y;
            // set selX, selY to new member.
        }
    }


    // set chessboard flags.
    void set_chessboard_flags()
    {
        if(blackturn == true) // if turn is blacks,
        {
            if(is_there_check(PlayerColor.white) == true)
            {
                check = true;
                if(is_there_checkmate(PlayerColor.white) == true)   //checkmate occur.
                {
                    checkmate = true;
                    end = true;
                    winner_is_black = true;
                }
            }
            //else if(is_there_check(PlayerColor.black) == true) check = true;
            else check = false;

            if(find_King(PlayerColor.white) == null)    // if there is no white king..
            {
                end = true;
                winner_is_black = true;
            }

            blackturn = false;
        }
        else
        {
            if(is_there_check(PlayerColor.black) == true)
            {
                check = true;
                if(is_there_checkmate(PlayerColor.black) == true) //checkmate occur.
                {
                    checkmate = true;
                    end = true;
                    winner_is_black = false;
                }
            }
            //else if(is_there_check(PlayerColor.white) == true) check = true;
            else check = false;

            if(find_King(PlayerColor.black) == null)  // if there is no black king..
            {
                end = true;
                winner_is_black = false;
            }

            blackturn = true;
        }

    }

    void onInitiateBoard(){
        blackturn = true;   // first turn is black's turn
        check = false;
        checkmate = false;
        end = false;
        winner_is_black = false;
        chessBoard_currentStatus = MagicType.NONE;
        print_chessboard_status();
    }

    void print_chessboard_status()
    {
        String msg = new String();

        if(blackturn == true)
        {
            msg = msg + "Now BLACK's turn ";
        }
        else
        {
            msg = msg + "Now WHITE's turn ";
        }

        if(check == true)
        {
            msg = msg + "| CHECK";
        }
        if(checkmate == true)
        {
            msg = msg + "MATE!";
        }

        if(end == true)
        {
            if(winner_is_black == true)
            {
                msg = msg + " WINNER is BLACK!";
            }
            else
            {
                msg = msg + " WINNER is WHITE!";
            }
        }

        setStatus(msg);
    }



    // for checking for checkmate, we need to simulate. simulate means that not change graphics, just change chessboard's pieces only.
    // this function get Piece to do action, and this piece do action at x, y location.
    // if action could be done, return value is acted piece. else, return null.
    // before simulate, user should save actor_piece's x and y location.
    private Piece do_simulate(Piece actor_piece, int x, int y)
    {
        Piece actedPiece = getPiece(x, y);

        if(actor_piece.canMove(x, y))
        {
            int actor_x, actor_y;
            actor_x = actor_piece.getX();
            actor_y = actor_piece.getY();
            chessBoardStatus[y][x] = actor_piece;
            chessBoardStatus[actor_y][actor_x] = new EmptyPiece(actor_x, actor_y);
            actor_piece.setLocation(x, y);
        }
        else if(actor_piece.canAttack(x, y))
        {
            int actor_x, actor_y;
            actor_x = actor_piece.getX();
            actor_y = actor_piece.getY();
            chessBoardStatus[y][x] = actor_piece;
            chessBoardStatus[actor_y][actor_x] = new EmptyPiece(actor_x, actor_y);
            actor_piece.setLocation(x, y);
        }
        else return null;

        return actedPiece;
    }

    // for checking for checkmate. this function undoing simulate. need to get actor's x and actor's y
    // this function must be called after do_simulate function.
    private void undo_simulate(Piece actor_piece, Piece acted_piece, int actor_x, int actor_y)
    {
        int acted_x, acted_y;
        acted_x = acted_piece.getX();
        acted_y = acted_piece.getY();

        chessBoardStatus[actor_y][actor_x] = actor_piece;
        actor_piece.setLocation(actor_x, actor_y);
        chessBoardStatus[acted_y][acted_x] = acted_piece;
    }

    // find king by color. if there is no king, return null.
    Piece find_King(PlayerColor color)
    {
        Piece curr;

        int i,j;
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                curr = getPiece(i, j);
                if(curr.getType() == PieceType.king && curr.getColor() == color)
                {
                    return curr;
                }
            }
        }

        return null;
    }

    // if colored king could be attacked, then return number of attacker. else, return 0.
    int colered_king_attacked_number(PlayerColor color)
    {
        Piece king = find_King(color);
        Piece curr;
        int i,j;
        int num_attacker = 0;

        if(king == null) return num_attacker;

        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                curr = getPiece(i, j);
                if(curr.canAttack(king.getX(), king.getY()) == true)    // if king could be attacked..
                {
                    num_attacker++;
                }
            }
        }
        return num_attacker;
    }

    boolean is_there_check(PlayerColor color)
    {
        if(colered_king_attacked_number(color) != 0) return true;
        else return false;
    }

    // if there is way to king move or attack to evade check, return true else return false.
    boolean if_king_could_move_to_evade_check(PlayerColor color)
    {
        Piece king = find_King(color);
        Piece acted_piece = null;
        int king_x = king.getX();
        int king_y = king.getY();
        int num_attacker;

        int i,j;

        for(i = -1; i <= 1; i++)
        {
            for(j = -1; j <= 1; j++)
            {
                if(i==0 && j==0) continue;
                acted_piece = do_simulate(king, king_x + i, king_y + j);
                if(acted_piece != null)
                {
                    num_attacker = colered_king_attacked_number(color);
                    if(num_attacker == 0 || acted_piece.getType() == PieceType.king) // if king can kill other king, it is not checkmate.
                    {
                        undo_simulate(king, acted_piece, king_x, king_y);
                        return true;
                    }
                    undo_simulate(king, acted_piece, king_x, king_y);
                }
            }
        }

        return false;
    }

    //if there is way to kill attacker not using our king.
    boolean if_could_kill_attacker(PlayerColor color)
    {
        Piece king = find_King(color);
        Piece curr;
        Piece attacker = null;
        int i,j;
        int num_attacker = 0;

        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                curr = getPiece(i, j);
                if(curr.canAttack(king.getX(), king.getY()) == true)    // if king could be attacked..
                {
                    attacker = curr;
                    num_attacker++;
                }
            }
        }

        if(attacker == null) return true;   // there is no attacker
        else if(num_attacker > 1) return false;  // there is no way to kill or block two attacker.
        else    // there is just one attacker.
        {
            for(i = 0; i < 8; i++)
            {
                for(j = 0; j < 8; j++)
                {
                    curr = getPiece(i, j);
                    if(curr.getType() != PieceType.king && curr.canAttack(attacker.getX(), attacker.getY()) == true)    // if attacker could be attacked.. (attacker shouldn't be king. king's attack only possible when king move.)
                    {
                        Piece acted_piece;
                        acted_piece = do_simulate(curr, attacker.getX(), attacker.getY());
                        if(acted_piece != null)
                        {
                            num_attacker = colered_king_attacked_number(color);
                            if(num_attacker == 0)
                            {
                                undo_simulate(curr, acted_piece, i, j);
                                return true;
                            }
                            undo_simulate(curr, acted_piece, i, j);
                        }
                    }
                }
            }   // there is way to kill attacker
        }
        return false;
    }

    //if there is way to block attacker not using our king.
    boolean if_could_block_attacker(PlayerColor color)
    {
        Piece king = find_King(color);
        Piece curr;
        Piece attacker = null;
        int king_x, king_y;
        int i,j;
        int num_attacker = 0;
        boolean[][] attack_route = new boolean [8][8]; // boolean that means if true, then if this way blocked, our king saved.

        king_x = king.getX();
        king_y = king.getY();

        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                attack_route[i][j] = false;
            }
        }
        // initialize attack_route.

        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                curr = getPiece(i, j);
                if(curr.canAttack(king_x, king_y) == true)    // if king could be attacked..
                {
                    attacker = curr;
                    num_attacker++;
                }
            }
        }

        if(attacker == null) return true;   // there is no attacker
        else if(num_attacker > 1) return false;  // there is no way to block two attacker.
        else    // there is just one attacker.
        {
            // find way to block attacker
            if(attacker.getType() == PieceType.pawn) // there is no way to block pawn
            {
                return false;
            }
            if(attacker.getType() == PieceType.knight) // there is no way to block knight
            {
                return false;
            }
            if(attacker.getType() == PieceType.king) // we can kill other king
            {
                return true;
            }
            if(attacker.getType() == PieceType.rook || attacker.getType() == PieceType.queen) // get rook's attack route.
            {
                // find attacker's attack route
                int attack_x = attacker.getX();
                int attack_y = attacker.getY();

                if(king_x == attack_x && king_y == attack_y)
                {
                    // do nothing.
                }
                else if(king_x != attack_x && king_y != attack_y) // no horizonal or vertical
                {
                    // do nothing.
                }
                else
                {
                    if(king_x < attack_x)  // left
                    {
                        for(i = attack_x - 1; i > king_x ; i--)
                        {
                            attack_route[i][king_y] = true;
                        }
                    }
                    else if(king_x > attack_x) // right
                    {
                        for(i = attack_x + 1; i < king_x ; i++)
                        {
                            attack_route[i][king_y] = true;
                        }
                    }
                    else if(king_y < attack_y) // upper
                    {
                        for(i = attack_y - 1; i > king_y ; i--)
                        {
                            attack_route[king_x][i] = true;
                        }
                    }
                    else if(king_y > attack_y) // upper
                    {
                        for(i = attack_y + 1; i < king_y ; i++)
                        {
                            attack_route[king_x][i] = true;
                        }
                    }
                }
            }
            if(attacker.getType() == PieceType.bishop || attacker.getType() == PieceType.queen) // get bishop's attack route.
            {
                int attack_x = attacker.getX();
                int attack_y = attacker.getY();

                if(king_x == attack_x && king_y == attack_y)
                {
                    // do nothing.
                }
                else if(Math.abs(king_x - attack_x) != Math.abs(king_y - attack_y)) // no diagonal
                {
                    // do nothing.
                }
                else
                {
                    if(king_x < attack_x)  // left
                    {
                        if(king_y < attack_y)  // left upper
                        {
                            i = attack_x - 1;
                            j = attack_y - 1;
                            while((i > king_x) && (j > king_y))
                            {
                                attack_route[i][j] = true;
                                i--;
                                j--;
                            }
                        }
                        else if(king_y > attack_y) // left lower
                        {
                            i = attack_x - 1;
                            j = attack_y + 1;
                            while((i > king_x) && (j < king_y))
                            {
                                attack_route[i][j] = true;
                                i--;
                                j++;
                            }
                        }
                    }
                    else if(king_x > attack_x) // right
                    {
                        if(king_y < attack_y)  // right upper
                        {
                            i = attack_x + 1;
                            j = attack_y - 1;
                            while((i > king_x) && (j > king_y))
                            {
                                attack_route[i][j] = true;
                                i++;
                                j--;
                            }
                        }
                        else if(king_y > attack_y) // right lower
                        {
                            i = attack_x + 1;
                            j = attack_y + 1;
                            while((i > king_x) && (j < king_y))
                            {
                                attack_route[i][j] = true;
                                i++;
                                j++;
                            }
                        }
                    }
                }
            }


            // now find all of attack route, then check if we could block these path.


            int k,l;

            for(i = 0; i < 8; i++)
            {
                for(j = 0; j < 8 ; j++)
                {
                    curr = getPiece(i, j);
                    if(curr.getType() != PieceType.king && curr.getColor() == king.getColor())  // get all of our pieces, not include king.
                    {
                        for(k = 0 ; k < 8; k++)
                        {
                            for(l = 0; l < 8; l++)
                            {
                                if(attack_route[k][l] == true)
                                {
                                    if(curr.canMove(k, l) == true)  //if current can move to attack route, simulate.
                                    {
                                        Piece acted_piece;
                                        acted_piece = do_simulate(curr, k, l);
                                        if(acted_piece != null)
                                        {
                                            num_attacker = colered_king_attacked_number(color);
                                            if(num_attacker == 0)
                                            {
                                                undo_simulate(curr, acted_piece, i, j);
                                                return true; // if we can block attack route, return true.
                                            }
                                            undo_simulate(curr, acted_piece, i, j);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return false;
    }

    // check is there checkmate.
    boolean is_there_checkmate(PlayerColor color)
    {
        Piece king = find_King(color);
        PlayerColor other_color = PlayerColor.none;

        if(king == null) return false; // if king is nowhere, not checkmate. just game end.

        if(color == PlayerColor.black) other_color = PlayerColor.white;
        else if(color == PlayerColor.white) other_color = PlayerColor.black;
        if(is_there_check(other_color) == true) return false; // is there is way to kill other color's king in our turn, this is not checkmate.

        int num_attacker = colered_king_attacked_number(color);

        if(num_attacker == 0) return false;    // there is no check
        else
        {
            if(if_king_could_move_to_evade_check(color) == true) return false;
            if(if_could_kill_attacker(color) == true) return false;
            if(if_could_block_attacker(color) == true) return false;
        }

        return true;
    }

    Piece getPiece(int x, int y)    //get Piece by location.
    {
        if((x >= 0 && x < 8) && (y >=0 && y < 8)) return getIcon(x, y);
        else return null;
    }

    void setPiece(int x, int y, Piece piece)
    {
        if((x >= 0 && x < 8) && (y >=0 && y < 8)) setIcon(x ,y, piece);
        else return;
    }

    class PawnPiece extends Piece
    {
        private boolean notMoved;
        PawnPiece(PlayerColor color, PieceType type, int x, int y)
        {
            super(color, type, x, y);
            notMoved = true;
        }

        @Override
        public void markTiles()
        {
            int i;
            int j;

            for(j = 0; j < 8; j++)
            {
                for(i = 0; i < 8; i++)
                {
                    if(canMove(i,j))
                    {
                        getPiece(i, j).mark();
                        markPosition(i, j);
                    }

                    if(canAttack(i, j))
                    {
                        getPiece(i, j).mark();
                        markPosition(i, j);
                    }
                }
            }
        }

        public void unmarkTiles()
        {
            int i;
            int j;

            for(j = 0; j < 8; j++)
            {
                for(i = 0; i < 8; i++)
                {
                    if(canMove(i,j))
                    {
                        getPiece(i, j).unmark();
                        unmarkPosition(i, j);
                    }

                    if(canAttack(i, j))
                    {
                        getPiece(i, j).unmark();
                        unmarkPosition(i, j);
                    }
                }
            }
        }

        public void doActionTo(int x, int y)
        {
            if(canAttack(x, y))
            {
                attack(x, y);
            }
            else if(canMove(x, y))
            {
                move(x, y);
            }
            else return;

            this.notMoved = false;
        }

        @Override
        public boolean canMove(int x, int y)
        {
            Piece otherPiece = getPiece(x, y);
            if(otherPiece == null) return false;    // if x, y position is not vaild, return false.
            if(otherPiece.getType() != PieceType.none) return false; // if x, y position has Piece, return false.
            else
            {
                if(this.getColor() == PlayerColor.black)
                {
                    if(this.notMoved == true) // first move case
                    {
                        if(x == (this.getX() + 1) && y == this.getY()) return true;
                        else
                        {
                            if(x == (this.getX() + 2) && y == this.getY())
                            {
                                Piece anotherPiece = getPiece(this.getX() + 1, this.getY());
                                // check there is no piece front of pawn.
                                if(anotherPiece == null) return false;
                                else if(anotherPiece.getType() != PieceType.none) return false;
                                else return true;
                            }
                            else return false;
                        }
                    }
                    else    // not first move
                    {
                        if(x == (this.getX() + 1) && y == this.getY()) return true;
                        else return false;
                    }
                }
                else if(this.getColor() == PlayerColor.white)
                {
                    if(this.notMoved == true) // first move case
                    {
                        if(x == (this.getX() - 1) && y == this.getY()) return true;
                        else
                        {
                            if(x == (this.getX() - 2) && y == this.getY())
                            {
                                Piece anotherPiece = getPiece(this.getX() - 1, this.getY());
                                // check there is no piece front of pawn.
                                if(anotherPiece == null) return false;
                                else if(anotherPiece.getType() != PieceType.none) return false;
                                else return true;
                            }
                            else return false;
                        }
                    }
                    else // not first move
                    {
                        if(x == (this.getX() - 1) && y == this.getY()) return true;
                        else return false;
                    }
                }
            }
            return false;
        }

        @Override
        public boolean canAttack(int x, int y)
        {
            Piece otherPiece = getPiece(x, y);
            if(otherPiece == null) return false;
            if(otherPiece.getType() == PieceType.none || otherPiece.getColor() == this.getColor()) return false;
            else
            {
                if(this.getColor() == PlayerColor.black)
                {
                    if(x == (this.getX() + 1) && y == (this.getY() - 1)) return true;
                    else if(x == (this.getX() + 1) && y == (this.getY() + 1)) return true;
                    else return false;
                }
                else if(this.getColor() == PlayerColor.white) {
                    if(x == (this.getX() - 1) && y == (this.getY() - 1)) return true;
                    else if(x == (this.getX() - 1) && y == (this.getY() + 1)) return true;
                    else return false;
                }
            }
            return false;
        }

        private void move(int x, int y)
        {
            setIcon(x, y, this);
            setIcon(this.getX(), this.getY(), new EmptyPiece(this.getX(), this.getY()));
            this.setLocation(x, y);
        }

        private void attack(int x, int y)
        {
            setIcon(x, y, this);
            setIcon(this.getX(), this.getY(), new EmptyPiece(this.getX(), this.getY()));
            this.setLocation(x, y);
        }
    }

    class KnightPiece extends Piece
    {
        KnightPiece(PlayerColor color, PieceType type, int x, int y)
        {
            super(color, type, x, y);
        }

        @Override
        public void markTiles()
        {
            int i;
            int j;

            for(j = 0; j < 8; j++)
            {
                for(i = 0; i < 8; i++)
                {
                    if(canMove(i,j))
                    {
                        getPiece(i, j).mark();
                        markPosition(i, j);
                    }

                    if(canAttack(i, j))
                    {
                        getPiece(i, j).mark();
                        markPosition(i, j);
                    }
                }
            }
        }

        public void unmarkTiles()
        {
            int i;
            int j;

            for(j = 0; j < 8; j++)
            {
                for(i = 0; i < 8; i++)
                {
                    if(canMove(i,j))
                    {
                        getPiece(i, j).unmark();
                        unmarkPosition(i, j);
                    }

                    if(canAttack(i, j))
                    {
                        getPiece(i, j).unmark();
                        unmarkPosition(i, j);
                    }
                }
            }
        }

        public void doActionTo(int x, int y)    //
        {
            if(canAttack(x, y))
            {
                attack(x, y);
            }
            else if(canMove(x, y))
            {
                move(x, y);
            }
            else return;
        }

        @Override
        public boolean canMove(int x, int y)
        {
            Piece otherPiece = getPiece(x, y);
            if(otherPiece == null) return false;    // if x, y position is not vaild, return false.
            if(otherPiece.getType() != PieceType.none) return false; // if x, y position has Piece, return false.
            else
            {
                if(knight_could_go(x, y)) return true;
            }
            return false;
        }

        @Override
        public boolean canAttack(int x, int y)
        {
            Piece otherPiece = getPiece(x, y);
            if(otherPiece == null) return false;    // if x, y position is not vaild, return false.
            if(otherPiece.getType() == PieceType.none || otherPiece.getColor() == this.getColor()) return false; // if x, y position has no Piece, or that piece is mine, return false.
            else
            {
                if(knight_could_go(x, y)) return true;
            }
            return false;
        }

        private boolean knight_could_go(int x, int y)
        {
            int this_x = this.getX();
            int this_y = this.getY();

            if(x == this_x - 2 && y == this_y + 1) return true;
            else if (x == this_x - 2 && y == this_y - 1) return true; // left
            else if (x == this_x + 2 && y == this_y + 1) return true;
            else if (x == this_x + 2 && y == this_y - 1) return true; // right
            else if (x == this_x - 1 && y == this_y + 2) return true;
            else if (x == this_x + 1 && y == this_y + 2) return true; // upper
            else if (x == this_x - 1 && y == this_y - 2) return true;
            else if (x == this_x + 1 && y == this_y - 2) return true; // lower
            else return false;
        }

        private void move(int x, int y)
        {
            setIcon(x, y, this);
            setIcon(this.getX(), this.getY(), new EmptyPiece(this.getX(), this.getY()));
            this.setLocation(x, y);
        }

        private void attack(int x, int y)
        {
            setIcon(x, y, this);
            setIcon(this.getX(), this.getY(), new EmptyPiece(this.getX(), this.getY()));
            this.setLocation(x, y);
        }
    }

    class RookPiece extends Piece
    {
        RookPiece(PlayerColor color, PieceType type, int x, int y)
        {
            super(color, type, x, y);
        }

        @Override
        public void markTiles()
        {
            int i;
            int j;

            for(j = 0; j < 8; j++)
            {
                for(i = 0; i < 8; i++)
                {
                    if(canMove(i,j))
                    {
                        getPiece(i, j).mark();
                        markPosition(i, j);
                    }

                    if(canAttack(i, j))
                    {
                        getPiece(i, j).mark();
                        markPosition(i, j);
                    }
                }
            }
        }

        public void unmarkTiles()
        {
            int i;
            int j;

            for(j = 0; j < 8; j++)
            {
                for(i = 0; i < 8; i++)
                {
                    if(canMove(i,j))
                    {
                        getPiece(i, j).unmark();
                        unmarkPosition(i, j);
                    }

                    if(canAttack(i, j))
                    {
                        getPiece(i, j).unmark();
                        unmarkPosition(i, j);
                    }
                }
            }
        }

        public void doActionTo(int x, int y)    //
        {
            if(canAttack(x, y))
            {
                attack(x, y);
            }
            else if(canMove(x, y))
            {
                move(x, y);
            }
            else return;
        }

        @Override
        public boolean canMove(int x, int y)
        {
            Piece otherPiece = getPiece(x, y);
            if(otherPiece == null) return false;    // if x, y position is not vaild, return false.
            if(otherPiece.getType() != PieceType.none) return false; // if x, y position has Piece, return false.
            else
            {
                if(rook_could_go(x, y)) return true;
            }
            return false;
        }

        @Override
        public boolean canAttack(int x, int y)
        {
            Piece otherPiece = getPiece(x, y);
            if(otherPiece == null) return false;
            if(otherPiece.getType() == PieceType.none || otherPiece.getColor() == this.getColor()) return false;  // if x, y position has no Piece, or that piece is mine, return false.
            else
            {
                if(rook_could_go(x, y)) return true;
            }
            return false;
        }

        private boolean rook_could_go(int x, int y)
        {
            int this_x = this.getX();
            int this_y = this.getY();
            int i, j;
            Piece otherPiece;

            if(x == this_x && y == this_y) return false;
            else if(x != this_x && y != this_y) return false;  // no horizonal or vertical
            else
            {
                if(x < this_x)  // left
                {
                    for(i = this_x - 1; i > x ; i--)
                    {
                        otherPiece = getPiece(i, this_y);
                        if(otherPiece.getType() != PieceType.none) return false;
                    }
                    return true;
                }
                else if(x > this_x) // right
                {
                    for(i = this_x + 1; i < x ; i++)
                    {
                        otherPiece = getPiece(i, this_y);
                        if(otherPiece.getType() != PieceType.none) return false;
                    }
                    return true;
                }
                else if(y < this_y) // upper
                {
                    for(i = this_y - 1; i > y ; i--)
                    {
                        otherPiece = getPiece(this_x, i);
                        if(otherPiece.getType() != PieceType.none) return false;
                    }
                    return true;
                }
                else if(y > this_y) // lower
                {
                    for(i = this_y + 1; i < y ; i++)
                    {
                        otherPiece = getPiece(this_x, i);
                        if(otherPiece.getType() != PieceType.none) return false;
                    }
                    return true;
                }
            }

            return false;
        }

        private void move(int x, int y)
        {
            setIcon(x, y, this);
            setIcon(this.getX(), this.getY(), new EmptyPiece(this.getX(), this.getY()));
            this.setLocation(x, y);
        }

        private void attack(int x, int y)
        {
            setIcon(x, y, this);
            setIcon(this.getX(), this.getY(), new EmptyPiece(this.getX(), this.getY()));
            this.setLocation(x, y);
        }
    }

    class BishopPiece extends Piece
    {
        BishopPiece(PlayerColor color, PieceType type, int x, int y)
        {
            super(color, type, x, y);
        }

        @Override
        public void markTiles()
        {
            int i;
            int j;

            for(j = 0; j < 8; j++)
            {
                for(i = 0; i < 8; i++)
                {
                    if(canMove(i,j))
                    {
                        getPiece(i, j).mark();
                        markPosition(i, j);
                    }

                    if(canAttack(i, j))
                    {
                        getPiece(i, j).mark();
                        markPosition(i, j);
                    }
                }
            }
        }

        public void unmarkTiles()
        {
            int i;
            int j;

            for(j = 0; j < 8; j++)
            {
                for(i = 0; i < 8; i++)
                {
                    if(canMove(i,j))
                    {
                        getPiece(i, j).unmark();
                        unmarkPosition(i, j);
                    }

                    if(canAttack(i, j))
                    {
                        getPiece(i, j).unmark();
                        unmarkPosition(i, j);
                    }
                }
            }
        }

        public void doActionTo(int x, int y)    //
        {
            if(canAttack(x, y))
            {
                attack(x, y);
            }
            else if(canMove(x, y))
            {
                move(x, y);
            }
            else return;
        }

        @Override
        public boolean canMove(int x, int y)
        {
            Piece otherPiece = getPiece(x, y);
            if(otherPiece == null) return false;    // if x, y position is not vaild, return false.
            if(otherPiece.getType() != PieceType.none) return false; // if x, y position has Piece, return false.
            else
            {
                if(bishop_could_go(x, y)) return true;
            }
            return false;
        }

        @Override
        public boolean canAttack(int x, int y)
        {
            Piece otherPiece = getPiece(x, y);
            if(otherPiece == null) return false;
            if(otherPiece.getType() == PieceType.none || otherPiece.getColor() == this.getColor()) return false;  // if x, y position has no Piece, or that piece is mine, return false.
            else
            {
                if(bishop_could_go(x, y)) return true;
            }
            return false;
        }

        private boolean bishop_could_go(int x, int y)
        {
            int this_x = this.getX();
            int this_y = this.getY();
            int i, j;
            Piece otherPiece;

            if(x == this_x && y == this_y) return false;
            else if(Math.abs(x - this_x) != Math.abs(y - this_y)) return false; // no diagonal
            else
            {
                if(x < this_x)  // left
                {
                    if(y < this_y)  // left upper
                    {
                        i = this_x - 1;
                        j = this_y - 1;
                        while((i > x) && (j > y))
                        {
                            otherPiece = getPiece(i, j);
                            if(otherPiece.getType() != PieceType.none) return false;
                            i--;
                            j--;
                        }
                        return true;
                    }
                    else if(y > this_y) // left lower
                    {
                        i = this_x - 1;
                        j = this_y + 1;
                        while((i > x) && (j < y))
                        {
                            otherPiece = getPiece(i, j);
                            if(otherPiece.getType() != PieceType.none) return false;
                            i--;
                            j++;
                        }
                        return true;
                    }
                }
                else if(x > this_x) // right
                {
                    if(y < this_y)  // right upper
                    {
                        i = this_x + 1;
                        j = this_y - 1;
                        while((i < x) && (j > y))
                        {
                            otherPiece = getPiece(i, j);
                            if(otherPiece.getType() != PieceType.none) return false;
                            i++;
                            j--;
                        }
                        return true;
                    }
                    else if(y > this_y) // right lower
                    {
                        i = this_x + 1;
                        j = this_y + 1;
                        while((i < x) && (j < y))
                        {
                            otherPiece = getPiece(i, j);
                            if(otherPiece.getType() != PieceType.none) return false;
                            i++;
                            j++;
                        }
                        return true;
                    }
                }
            }
            return false;
        }

        private void move(int x, int y)
        {
            setIcon(x, y, this);
            setIcon(this.getX(), this.getY(), new EmptyPiece(this.getX(), this.getY()));
            this.setLocation(x, y);
        }

        private void attack(int x, int y)
        {
            setIcon(x, y, this);
            setIcon(this.getX(), this.getY(), new EmptyPiece(this.getX(), this.getY()));
            this.setLocation(x, y);
        }
    }

    class QueenPiece extends Piece
    {
        QueenPiece(PlayerColor color, PieceType type, int x, int y)
        {
            super(color, type, x, y);
        }

        @Override
        public void markTiles()
        {
            int i;
            int j;

            for(j = 0; j < 8; j++)
            {
                for(i = 0; i < 8; i++)
                {
                    if(canMove(i,j))
                    {
                        getPiece(i, j).mark();
                        markPosition(i, j);
                    }

                    if(canAttack(i, j))
                    {
                        getPiece(i, j).mark();
                        markPosition(i, j);
                    }
                }
            }
        }

        public void unmarkTiles()
        {
            int i;
            int j;

            for(j = 0; j < 8; j++)
            {
                for(i = 0; i < 8; i++)
                {
                    if(canMove(i,j))
                    {
                        getPiece(i, j).unmark();
                        unmarkPosition(i, j);
                    }

                    if(canAttack(i, j))
                    {
                        getPiece(i, j).unmark();
                        unmarkPosition(i, j);
                    }
                }
            }
        }

        public void doActionTo(int x, int y)    //
        {
            if(canAttack(x, y))
            {
                attack(x, y);
            }
            else if(canMove(x, y))
            {
                move(x, y);
            }
            else return;
        }

        @Override
        public boolean canMove(int x, int y)
        {
            Piece otherPiece = getPiece(x, y);
            if(otherPiece == null) return false;    // if x, y position is not vaild, return false.
            if(otherPiece.getType() != PieceType.none) return false; // if x, y position has Piece, return false.
            else
            {
                if(rook_could_go(x, y) || bishop_could_go(x, y)) return true;
            }
            return false;
        }

        @Override
        public boolean canAttack(int x, int y)
        {
            Piece otherPiece = getPiece(x, y);
            if(otherPiece == null) return false;
            if(otherPiece.getType() == PieceType.none || otherPiece.getColor() == this.getColor()) return false;  // if x, y position has no Piece, or that piece is mine, return false.
            else
            {
                if(rook_could_go(x, y) || bishop_could_go(x, y)) return true;
            }
            return false;
        }

        private boolean rook_could_go(int x, int y)
        {
            int this_x = this.getX();
            int this_y = this.getY();
            int i, j;
            Piece otherPiece;

            if(x == this_x && y == this_y) return false;
            else if(x != this_x && y != this_y) return false;  // no horizonal or vertical
            else
            {
                if(x < this_x)  // left
                {
                    for(i = this_x - 1; i > x ; i--)
                    {
                        otherPiece = getPiece(i, this_y);
                        if(otherPiece.getType() != PieceType.none) return false;
                    }
                    return true;
                }
                else if(x > this_x) // right
                {
                    for(i = this_x + 1; i < x ; i++)
                    {
                        otherPiece = getPiece(i, this_y);
                        if(otherPiece.getType() != PieceType.none) return false;
                    }
                    return true;
                }
                else if(y < this_y) // upper
                {
                    for(i = this_y - 1; i > y ; i--)
                    {
                        otherPiece = getPiece(this_x, i);
                        if(otherPiece.getType() != PieceType.none) return false;
                    }
                    return true;
                }
                else if(y > this_y) // lower
                {
                    for(i = this_y + 1; i < y ; i++)
                    {
                        otherPiece = getPiece(this_x, i);
                        if(otherPiece.getType() != PieceType.none) return false;
                    }
                    return true;
                }
            }

            return false;
        }

        private boolean bishop_could_go(int x, int y)
        {
            int this_x = this.getX();
            int this_y = this.getY();
            int i, j;
            Piece otherPiece;

            if(x == this_x && y == this_y) return false;
            else if(Math.abs(x - this_x) != Math.abs(y - this_y)) return false; // no diagonal
            else
            {
                if(x < this_x)  // left
                {
                    if(y < this_y)  // left upper
                    {
                        i = this_x - 1;
                        j = this_y - 1;
                        while((i > x) && (j > y))
                        {
                            otherPiece = getPiece(i, j);
                            if(otherPiece.getType() != PieceType.none) return false;
                            i--;
                            j--;
                        }
                        return true;
                    }
                    else if(y > this_y) // left lower
                    {
                        i = this_x - 1;
                        j = this_y + 1;
                        while((i > x) && (j < y))
                        {
                            otherPiece = getPiece(i, j);
                            if(otherPiece.getType() != PieceType.none) return false;
                            i--;
                            j++;
                        }
                        return true;
                    }
                }
                else if(x > this_x) // right
                {
                    if(y < this_y)  // right upper
                    {
                        i = this_x + 1;
                        j = this_y - 1;
                        while((i < x) && (j > y))
                        {
                            otherPiece = getPiece(i, j);
                            if(otherPiece.getType() != PieceType.none) return false;
                            i++;
                            j--;
                        }
                        return true;
                    }
                    else if(y > this_y) // right lower
                    {
                        i = this_x + 1;
                        j = this_y + 1;
                        while((i < x) && (j < y))
                        {
                            otherPiece = getPiece(i, j);
                            if(otherPiece.getType() != PieceType.none) return false;
                            i++;
                            j++;
                        }
                        return true;
                    }
                }
            }
            return false;
        }

        private void move(int x, int y)
        {
            setIcon(x, y, this);
            setIcon(this.getX(), this.getY(), new EmptyPiece(this.getX(), this.getY()));
            this.setLocation(x, y);
        }

        private void attack(int x, int y)
        {
            setIcon(x, y, this);
            setIcon(this.getX(), this.getY(), new EmptyPiece(this.getX(), this.getY()));
            this.setLocation(x, y);
        }
    }

    class KingPiece extends Piece
    {
        KingPiece(PlayerColor color, PieceType type, int x, int y)
        {
            super(color, type, x, y);
        }

        @Override
        public void markTiles()
        {
            int i;
            int j;

            for(j = 0; j < 8; j++)
            {
                for(i = 0; i < 8; i++)
                {
                    if(canMove(i,j))
                    {
                        getPiece(i, j).mark();
                        markPosition(i, j);
                    }

                    if(canAttack(i, j))
                    {
                        getPiece(i, j).mark();
                        markPosition(i, j);
                    }
                }
            }
        }

        public void unmarkTiles()
        {
            int i;
            int j;

            for(j = 0; j < 8; j++)
            {
                for(i = 0; i < 8; i++)
                {
                    if(canMove(i,j))
                    {
                        getPiece(i, j).unmark();
                        unmarkPosition(i, j);
                    }

                    if(canAttack(i, j))
                    {
                        getPiece(i, j).unmark();
                        unmarkPosition(i, j);
                    }
                }
            }
        }

        public void doActionTo(int x, int y)    //
        {
            if(canAttack(x, y))
            {
                attack(x, y);
            }
            else if(canMove(x, y))
            {
                move(x, y);
            }
            else return;
        }

        @Override
        public boolean canMove(int x, int y)
        {
            Piece otherPiece = getPiece(x, y);
            if(otherPiece == null) return false;    // if x, y position is not vaild, return false.
            if(otherPiece.getType() != PieceType.none) return false; // if x, y position has Piece, return false.
            else
            {
                if(king_could_go(x, y)) return true;
            }
            return false;
        }

        @Override
        public boolean canAttack(int x, int y)
        {
            Piece otherPiece = getPiece(x, y);
            if(otherPiece == null) return false;
            if(otherPiece.getType() == PieceType.none || otherPiece.getColor() == this.getColor()) return false;  // if x, y position has no Piece, or that piece is mine, return false.
            else
            {
                if(king_could_go(x, y)) return true;
            }
            return false;
        }

        private boolean king_could_go(int x, int y)
        {
            int this_x = this.getX();
            int this_y = this.getY();

            if(x == this_x - 1 && y == this_y - 1) return true; // left upper
            else if (x == this_x && y == this_y - 1) return true; // upper
            else if (x == this_x + 1 && y == this_y - 1) return true; // right upper
            else if (x == this_x + 1 && y == this_y) return true; // right
            else if (x == this_x + 1 && y == this_y + 1) return true; // right lower
            else if (x == this_x && y == this_y + 1) return true; // lower
            else if (x == this_x - 1 && y == this_y + 1) return true; // left lower
            else if (x == this_x - 1 && y == this_y) return true; // left
            else return false;
        }

        private void move(int x, int y)
        {
            setIcon(x, y, this);
            setIcon(this.getX(), this.getY(), new EmptyPiece(this.getX(), this.getY()));
            this.setLocation(x, y);
        }

        private void attack(int x, int y)
        {
            setIcon(x, y, this);
            setIcon(this.getX(), this.getY(), new EmptyPiece(this.getX(), this.getY()));
            this.setLocation(x, y);
        }
    }

    class EmptyPiece extends Piece  // this piece doesn't do anything. just empty space.
    {
        EmptyPiece(int x, int y)
        {
            super(x, y);
        }

    }

}