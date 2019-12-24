#include "json.h"
#include <any>
using namespace std;
int main() {
	int g = 0;
	string s = "{\"lastname\" : \"Ivanov\",\"firstname\" : \"Ivan\",\"age\" : 25,\"islegal\" : false,\"marks\" : [ 4, 5, 5, 5, 2, 3] ,\"address\" : {\"city\" : \"Moscow\",\"street\" : \"Vozdvijenka\"}} ";
	JSON a;
	a.parse(s, g);
	cout << any_cast<string>(any_cast<JSON>(a["address"])["city"]) << endl;
	cout << any_cast<double>(any_cast<JSON>(a["marks"])[2]) << endl;
	system("pause");
	return 0;
}