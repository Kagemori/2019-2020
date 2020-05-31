class People {
	private String name;
	People(){}
	People(String n){name=n;}
	String getName(){
		return name;
	}
	void print() {
		System.out.println("Name:" + getName());
	}
}

class Employee extends People {
	private float salary;
	Employee() {}
	Employee(float s, String n) {
		super(n);
		salary=s;
	}
	void print() {
		super.print();
		System.out.println("Salary:" + salary);
	}	
}

class Faculty extends Employee {
	private String rank;
	Faculty () {}
	Faculty (String r, String n, float s) {
		super(s,n);
		rank=r;
	}
	void print() {
		super.print();
		System.out.println("Rank:" +rank);
	}
}

class Staff extends Employee {
	Staff(){}
	Staff(String n, float s) {
		super(s,n);
	}
	
}

class Student extends People {
	float tuition;
	Student() {}
	Student(float t, String n) {
		super(n);
		tuition=t;
	}
	float getTuition(){return tuition;}
	
}

class Undergraduate extends Student {
	String major;
	Undergraduate(){
	}
	Undergraduate(String m, String n, float t) {
		super(t,n);
		major=m;
	}

	void print() {
		System.out.println("Name: " + super.getName() +
				", tuition: " +super.getTuition() + 
				", Major: " + major);
	}
}

class Graduate extends Student {
	String program;
	Graduate(){
	}
	Graduate(String p, String n, float t) {
		super(t,n);
		program=p;
	}
}

public class OOPDesign {
  public static void main(String args[]) {
	Undergraduate u1= new Undergraduate("CS","A",5000);
	Graduate g1=new Graduate("MIS","B",7000);
	Faculty f1=new Faculty("Assistant Professor","C",20000);
	Staff s1=new Staff("D",20000);
	u1.print();
	g1.print();
	f1.print();
	s1.print();
  }
}
