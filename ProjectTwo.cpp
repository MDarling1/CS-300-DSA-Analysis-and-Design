//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Michelle Darling
// Version     : 1.0
// Description : CS-300 Project 2, creating a cursed program for 
//               ABCU course organization
// Tested with : ABCU_Course_List.txt, ABCU_Course_List.csv
//============================================================================
// 
//============================================================================
// All the usual #include shenanigans for i/o streams, file streams, 
// string streams, regular expressions, etc.
//============================================================================
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

//============================================================================
// Definitions for the Course object, Node structure, and Courses objects
//============================================================================

class Course {
	// Private variables for Course objects
private:
	string courId;
	string courName;
	vector<string> courPrereq;
	// Public functions for Course objects
public:
	Course() = default;
	Course(string& id, string& name, vector<string>& prereq);
	string getCourseId();
	string getCourseName();
	vector<string> getCoursePrereq();
	string courseToString();
};

struct Node {
	Course course;
	Node* left;
	Node* right;

	// Default Node constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// Initialize Course
	Node(Course newCourse) :
		Node() {
		course = newCourse;
	}
};

class Courses {
	// Private functions for Courses
private:
	Node* root;
	void addNode(Node* node, Course course);
	void courseSort(Node* node);
	// Public methods for Courses
public:
	Courses();
	void CourseSort();
	void Insert(Course course);
	Course Search(string courseId);
};

//============================================================================
// Default Constructors
//============================================================================

// Constructor to create Course object
Course::Course(string& id, string& name, vector<string>& prereq) {
	this->courId = id;
	this->courName = name;
	this->courPrereq = prereq;
}

Courses::Courses() {
	// root is equal to nullptr
	root = nullptr;
}

// Accessor for course ID
string Course::getCourseId() {
	return this->courId;
}

// Accessor for course name
string Course::getCourseName() {
	return this->courName;
}

// Accessor for the list of course prerequisites
vector<string> Course::getCoursePrereq() {
	return this->courPrereq;
}

// Creates a string of course values ID and name
string Course::courseToString() {
	return this->courId + ", " + this->courName;
}

// Calls the inOrder function, passing root
void Courses::CourseSort() {
	courseSort(root);
}

//============================================================================
// Functions and fun things
//============================================================================

// Function to load and read file, and creates a BST to place Courses in
void loadFile(const string& inputFile, Courses* courseTree) {
	cout << "Loading file " << inputFile << " . . ." << endl;

	// Variables for input file stream, file; string, currentLine
	ifstream file;
	string currentLine;
	try {
		// Open the given file
		file.open(inputFile);
		while (getline(file, currentLine)) {
			stringstream ss(currentLine);
			string word, id, name;
			vector<string> prereq;
			int index = 0;
			while (!ss.eof()) {
				getline(ss, word, ',');
				word = regex_replace(word, regex(R"(\r\n|\r|\n)"), "");
				if (index == 0) {
					id = word;
				}

				else if (index == 1) {
					name = word;
				}

				else {
					prereq.push_back(word);
				}

				index++;
			}

			// Creates a BST and inserts Courses into the tree
			Course course = Course(id, name, prereq);
			courseTree->Insert(course);
		}
	}

	catch (ifstream::failure& e) {
		cerr << e.what() << endl;
	}

	// Close the given file
	file.close();
}

// Function to insert a course into the tree
void Courses::Insert(Course course) {
	// If root is equal to nullptr
	if (root == nullptr) {
		// root is equal to new node course
		root = new Node(course);
	}

	// Else
	else {
		// This node calls the addNode function, passing root and course
		this->addNode(root, course);
	}
}

// Function to add a course node
void Courses::addNode(Node* node, Course course) {
	// If node is smaller then add to left
	if (node != nullptr && (node->course.getCourseId().compare(course.getCourseId()) > 0)) {
		// If no left node
		if (node->left == nullptr) {
			// This node becomes left node
			node->left = new Node(course);
			return;
		}

		// Else recurse down the left side
		else {
			this->addNode(node->left, course);
		}
	}

	// Else
	else if (node != nullptr && (node->course.getCourseId().compare(course.getCourseId()) < 0)) {
		// If no right node
		if (node->right == nullptr) {
			// This node becomes right node
			node->right = new Node(course);
			return;
		}

		// Else recurse down the right side
		else {
			this->addNode(node->right, course);
		}
	}
}

// Function to sort courses
void Courses::courseSort(Node* node) {
	// If node is not equal to null ptr
	if (node != nullptr) {
		// inOrder left
		courseSort(node->left);
		// Output courseID and courseName
		cout << node->course.getCourseId() << ", "
			<< node->course.getCourseName() << endl;
		// inOrder right
		courseSort(node->right);
	}
}

// Function to display courses
void displayCourse(Course course) {
	vector<string> coursePrereq = course.getCoursePrereq();
	string prereq;
	// Prints the singular prerequisite
	if (coursePrereq.size() == 1) {
		prereq = course.getCoursePrereq()[0];
	}

	// For all prerequisites, print each one
	else if (coursePrereq.size() > 1) {
		for (int i = 0; i < coursePrereq.size() - 1; i++) {
			prereq += coursePrereq[i] + ", ";
		}
		prereq += coursePrereq.back();
	}

	// Else when no prerequisites, prints N/A
	else {
		prereq = "N/A";
	}

	// Prints out the course details
	cout << course.getCourseId() << ", " << course.getCourseName() << endl;
	cout << "Prerequisites: " << prereq << endl;
}

// Search for a specific course by course ID
Course Courses::Search(string courseId) {
	// Transforms each input course ID character to uppercase
	transform(courseId.begin(), courseId.end(), courseId.begin(), ::toupper);
	// Set current node equal to root
	Node* cur = root;
	// Loop through courses until either matching course ID found or end of tree
	while (cur != nullptr) {
		// If match found, return course
		if (cur->course.getCourseId().compare(courseId) == 0) {
			return cur->course;
		}

		// If course ID is smaller than current node, traverse left
		else if (courseId.compare(cur->course.getCourseId()) < 0) {
			cur = cur->left;
		}

		// Else course ID is larger, traverse right
		else {
			cur = cur->right;
		}
	}

	// Course not found, return empty
	Course course;
	return course;
}

//============================================================================
// The one, the only, The Main Method
//============================================================================

int main() {
	// Define a binary search tree to hold all courses
	Courses* courseTree;
	courseTree = new Courses();
	Course course;
	string inputFile, courseKey;

	cout << "Welcome to the ABCU Course Planner!" << endl << endl;

	int option = 0;
	while (option != 9) {
		cout << "Please choose one of the following options:" << endl;
		cout << "  1. Load Course List File" << endl;
		cout << "  2. Print Course List" << endl;
		cout << "  3. Print Course" << endl;
		cout << "  9. Exit" << std::endl << endl;
		cin >> option;

		switch (option) {
		case 1:

			cout << "Enter the file name: ";
			cin >> inputFile;
			// Call the loadFile function, passing the entered file and BST
			loadFile(inputFile, courseTree);
			break;

		case 2:
			cout << "Here is a list of all courses:" << endl << endl;
			// Calls CourseSort() for courseTree
			courseTree->CourseSort();
			break;

		case 3:
			cout << "Please enter the course ID: " << endl;
			cin >> courseKey;
			// course is equal to the searched courseKey in the courseTree
			course = courseTree->Search(courseKey);

			if (!course.getCourseId().empty()) {
				displayCourse(course);
			}

			else {
				cout << "Course ID " << courseKey << " was not found." << endl;
			}

			break;

		case 9:
			cout << "Thank you for using the ABCU Course Planner. Goodbye!" << endl;
			break;

		default:
			cout << option << " is not a valid option. Please try again." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		}

		cout << endl;
	}

	return 0;
}