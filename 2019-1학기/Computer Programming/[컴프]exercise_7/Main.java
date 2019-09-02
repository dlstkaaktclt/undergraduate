import java.io.*;
import java.util.*;

interface Repulsor {
    int repulsorDamage();	//have initial health +0 | deals additional 5 damage
}

interface FireBlaster {
    int fireBlasterDamage();//have initial health +0 | deals additional 15 damage, but damaged 5 health
}

interface WhipFlash {
    int whipFlashDamage();	//have initial health -30 | deals additional 3 damage, and heals 3 health
}

interface HulkBuster {
    int hulkBusterDamage();	//have initial health +50 | deals additional -2 damage
}


class Ironman {
    protected String name;
    protected int health;

    public Ironman(){}

    public Ironman(String name) {
        this.name = name;
        setHealth();
    }

    public String getName() {
        return name;
    }

    public int deal() {
        return 5;
    }

    protected void setHealth() {
        this.health = 50;
    }

    public void damaged(int damage) {
        this.health -= damage;
    }

    public boolean isDead() {
        return health <= 0;
    }

    @Override
    public String toString() {
        return name + "[" + health + "]";
    }
}


// TODO
class IronmanRepulsor extends Ironman implements Repulsor {

    private String type;

    public IronmanRepulsor(){
        super();
        this.type = "Repulsor";
    }
    public IronmanRepulsor(String name) {
        super(name);
        this.type = "Repulsor";
    }

    @Override
    public int deal() { return super.deal() + this.repulsorDamage(); }

    @Override
    public int repulsorDamage()
    {
        return 5;
    }

    @Override
    public String toString() {
        return super.toString() + "_" + "ArmorType" + "[" + this.type + "]";
    }

}


// TODO
class IronmanFireBlaster extends Ironman implements FireBlaster {

    private String type;

    public IronmanFireBlaster(){
        super();
        this.type = "FireBlaster";
    }
    public IronmanFireBlaster(String name) {
        super(name);
        this.type = "FireBlaster";
    }

    @Override
    public int deal() {
        return super.deal() + this.fireBlasterDamage();
    }

    @Override
    public int fireBlasterDamage()
    {
        this.damaged(5);
        return 15;
    }

    @Override
    public String toString() {
        return super.toString() + "_" + "ArmorType" + "[" + this.type + "]";
    }
}


// TODO
class IronmanWhipFlash extends Ironman implements WhipFlash {

    private String type;

    public IronmanWhipFlash(){
        super();
        this.type = "WhipFlash";
    }
    public IronmanWhipFlash(String name) {
        super(name);
        this.type = "WhipFlash";
    }

    @Override
    protected void setHealth() {
        super.setHealth();
        this.health -= 30;
    }

    @Override
    public int whipFlashDamage()
    {
        this.heal(3);
        return 3;
    }

    @Override
    public int deal() {
        return super.deal() + this.whipFlashDamage();
    }

    private void heal(int healpoint) {
        this.health += healpoint;
    }

    @Override
    public String toString() {
        return super.toString() + "_" + "ArmorType" + "[" + this.type + "]";
    }
}


// TODO
class IronmanRepulsorFireBlaster extends Ironman implements Repulsor, FireBlaster{

    private String type1;
    private String type2;

    public IronmanRepulsorFireBlaster(){
        super();
        this.type1 = "Repulsor";
        this.type2 = "FireBlaster";
    }
    public IronmanRepulsorFireBlaster(String name) {
        super(name);
        this.type1 = "Repulsor";
        this.type2 = "FireBlaster";
    }
    @Override
    public int deal() {
        return super.deal() + this.repulsorDamage() + this.fireBlasterDamage();
    }

    @Override
    public int repulsorDamage()
    {
        return 15;
    }

    @Override
    public int fireBlasterDamage()
    {
        this.damaged(10);
        return 15;
    }

    @Override
    public String toString() {
        return super.toString() + "_" + "ArmorType" + "[" + this.type1 + "]" + "[" + this.type2 + "]";
    }

}


// TODO
class IronmanRepulsorWhipFlash extends Ironman implements Repulsor, WhipFlash {

    private String type1;
    private String type2;

    public IronmanRepulsorWhipFlash(){
        super();
        this.type1 = "Repulsor";
        this.type2 = "WhipFlash";
    }
    public IronmanRepulsorWhipFlash(String name) {
        super(name);
        this.type1 = "Repulsor";
        this.type2 = "WhipFlash";
    }

    @Override
    public int deal() {
        return super.deal() + this.whipFlashDamage() + this.repulsorDamage();
    }

    @Override
    public int repulsorDamage()
    {
        return 10;
    }

    @Override
    public int whipFlashDamage()
    {
        return 3;
    }

    @Override
    public String toString() {
        return super.toString() + "_" + "ArmorType" + "[" + this.type1 + "]" + "[" + this.type2 + "]";
    }
}


// TODO
class IronmanHulkBuster extends Ironman implements HulkBuster {

    private String type;

    public IronmanHulkBuster(){
        super();
        this.type = "HulkBuster";
    }
    public IronmanHulkBuster(String name) {
        super(name);
        this.type = "HulkBuster";
    }
    @Override
    protected void setHealth() {
        super.setHealth();
        this.health += 50;
    }

    @Override
    public int hulkBusterDamage()
    {
        return -2;
    }

    @Override
    public int deal() {
        return super.deal() + this.hulkBusterDamage();
    }

    @Override
    public String toString() {
        return super.toString() + "_" + "ArmorType" + "[" + this.type + "]";
    }
}






public class Main {
    private final static int MAX_FIELD_SIZE = 5;
    private static ArrayList<Ironman> armyList = new ArrayList<>();

    private static String printEnemyHealth(int enemyHealth) {
        return "Enemy Health : " + enemyHealth;
    }

    private static String printMyHealth(int myHealth) {
        return "My Health : " + myHealth;
    }

    private final static String ACTION_STR =
            "====Select Action====\n"
                    + "1. Add Army\n"
                    + "2. Deal Enemy\n"
                    + "3. Quit\n"
                    + "=====================\n>>>";
    private final static String ARMOR_LIST_STR =
            "=======Armor List========\n"
                    + "0. Basic\n"
                    + "1. Repulsor\n"
                    + "2. Whip Flash\n"
                    + "3. Fire Blaster\n"
                    + "4. Repulsor & Whip Flash\n"
                    + "5. Repulsor & Fire Blaster\n"
                    + "6. Hulk Buster\n"
                    + "=========================\n>>>";

    private static void summon(int summonTarget, String name) {
        switch (summonTarget) {
            case 0:
                armyList.add(new Ironman(name));
                break;
            case 1:
                armyList.add(new IronmanRepulsor(name));
                break;
            case 2:
                armyList.add(new IronmanWhipFlash(name));
                break;
            case 3:
                armyList.add(new IronmanFireBlaster(name));
                break;
            case 4:
                armyList.add(new IronmanRepulsorWhipFlash(name));
                break;
            case 5:
                armyList.add(new IronmanRepulsorFireBlaster(name));
                break;
            case 6:
                armyList.add(new IronmanHulkBuster(name));
                break;
        }
    }


    private static int deal() {
        int totalDealing = 0;
        for (Ironman a : armyList) {
            totalDealing += a.deal();
        }
        return totalDealing;
    }

    private static int damaged(int myHealth) {
        if (armyList.isEmpty()) {
            return myHealth - 20;
        }
        else {
            for (Ironman armor : armyList) {
                armor.damaged(10);
            }
            return myHealth;
        }
    }

    private static String printAllArmy() {
        String ret = "====All Army List====\n";
        for (Ironman armor : armyList) {
            ret += armor + "\n";
        }
        ret += "=====================";
        return ret;
    }


    public static void main(String[] args) {
        int enemyHealth = 200;
        int myHealth = 200;

        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        try {
            while (true) {
                System.out.println(printEnemyHealth(enemyHealth));
                System.out.println(printMyHealth(myHealth));
                System.out.println(printAllArmy());
                System.out.print(ACTION_STR);

                int input = Integer.parseInt(br.readLine());

                switch (input) {
                    case 3:
                        return;

                    case 1:
                        if (armyList.size() >= MAX_FIELD_SIZE) {
                            System.out.println("Field is full");
                            continue;
                        }
                        else {
                            System.out.print(ARMOR_LIST_STR);

                            int summon_target = Integer.parseInt(br.readLine());
                            System.out.print("Name: ");
                            String name = br.readLine();

                            summon(summon_target, name);

                            System.out.println("New Armor Added!");
                        }
                        break;

                    case 2:
                        int d = deal();
                        enemyHealth -= d;
                        System.out.println("Total " + Integer.toString(d) + " damage dealed");
                        break;

                    default:
                        System.out.println("WRONG INPUT");
                        continue;
                }
                myHealth = damaged(myHealth);

                ArrayList<Ironman> removeList = new ArrayList<>();

                for (Ironman armor : armyList) {
                    if (armor.isDead()) {
                        System.out.println(armor.getName() + " has fallen");
                        removeList.add(armor);
                    }
                }

                armyList.removeAll(removeList);

                if (enemyHealth <= 0) {
                    System.out.println("YOU WIN!");
                    break;
                }
                else if (myHealth <= 0) {
                    System.out.println("YOU LOSE..");
                    break;
                }
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }
}
