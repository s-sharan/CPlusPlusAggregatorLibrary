#include <iostream>
#include "aggregator.h"
#include "graphexecutor.h"
#include <unordered_set>
#include <thread>

using namespace AggregatorService;
using namespace GraphExecutorService;
using namespace std;

mutex m;
class TestHashSet {
private:
	unordered_set<string> strings;
public:
	void addTasks(string s) {
		strings.insert(s);
	}
	unordered_set<string> getTasks() {
		return strings;
	}
	void printTasks() {
		for(auto s : strings) {
			cout << s << endl;
		}
	}
};

class A : public Runnable {
private:
	string label{"A"};
	unordered_set<string> dependencies;
public:
	string getLabel() const {
		return label;
	}

	unordered_set<string> getDependencies() const {
		return dependencies;
	}

	void run(unordered_map<string, shared_ptr<Runnable>> dependencies) {
		cout << "Running node" << label << endl;
	}
};

class B : public Runnable {
private:
	string label{"B"};
	unordered_set<string> dependencies{"A"};
public:
	string getLabel() const {
		return label;
	}

	unordered_set<string> getDependencies() const {
		return dependencies;
	}

	void run(unordered_map<string, shared_ptr<Runnable>> dependencies) {
		cout << "Running node" << label << " with dependencies ";
		for(auto it = dependencies.begin(); it != dependencies.end(); it++) {
			shared_ptr<Runnable> node = it->second;
			cout << node->getLabel() << " ";
		}
		cout << endl;
	}
};

class C : public Runnable {
private:
	string label{"C"};
	unordered_set<string> dependencies{"A"};
public:
	string getLabel() const {
		return label;
	}

	unordered_set<string> getDependencies() const {
		return dependencies;
	}

	void run(unordered_map<string, shared_ptr<Runnable>> dependencies) {
		cout << "Running node" << label << " with dependencies ";
				for(auto it = dependencies.begin(); it != dependencies.end(); it++) {
					shared_ptr<Runnable> node = it->second;
					cout << node->getLabel() << " ";
				}
				cout << endl;
	}
};

class D : public Runnable {
private:
	string label{"D"};
	unordered_set<string> dependencies{"B", "C"};
public:
	string getLabel() const {
		return label;
	}

	unordered_set<string> getDependencies() const {
		return dependencies;
	}

	void run(unordered_map<string, shared_ptr<Runnable>> dependencies) {
		cout << "Running node" << label << " with dependencies ";
				for(auto it = dependencies.begin(); it != dependencies.end(); it++) {
					shared_ptr<Runnable> node = it->second;
					cout << node->getLabel() << " ";
				}
				cout << endl;
	}
};
class E : public Runnable {
private:
	string label{"E"};
	unordered_set<string> dependencies{"D"};
public:
	string getLabel() const {
		return label;
	}

	unordered_set<string> getDependencies() const {
		return dependencies;
	}

	void run(unordered_map<string, shared_ptr<Runnable>> dependencies) {
		cout << "Running node" << label << " with dependencies ";
				for(auto it = dependencies.begin(); it != dependencies.end(); it++) {
					shared_ptr<Runnable> node = it->second;
					cout << node->getLabel() << " ";
				}
				cout << endl;
	}
};

int main() {
	shared_ptr<Aggregator> aggregator = AggregatorFactory::newFixedThreadPoolAggregator();
	aggregator->addNode(shared_ptr<Runnable>(new A()));
	aggregator->addNode(shared_ptr<Runnable>(new B()));
	aggregator->addNode(shared_ptr<Runnable>(new C()));
	aggregator->addNode(shared_ptr<Runnable>(new D()));
//	shared_ptr<Aggregator> aggregator2 = AggregatorFactory::newFixedThreadPoolAggregator();
//	aggregator2->addNode(shared_ptr<Runnable>(new A()));
//	aggregator2->addNode(shared_ptr<Runnable>(new B()));
//	aggregator2->addNode(shared_ptr<Runnable>(new C()));
//	aggregator2->addNode(shared_ptr<Runnable>(new D()));
	aggregator->execute();
	aggregator->addNode(shared_ptr<Runnable>(new E()));
	aggregator->execute();
	cout<<"Finished executing \n";
	return 0;
}
