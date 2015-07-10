#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

struct RoadEdge {
	int dest, curTime, minTime;
};

struct CityNode {
	int dest, moneySpent;
};

int n, money, myMoney;
vector<RoadEdge> adj[100002];
int low, high, maxCost[100002];
bool visited[100002];

void adjustLowHighDFS(RoadEdge cur){

	low = max(low, cur.minTime);
	high = max(high, cur.curTime);
	maxCost[cur.dest] = cur.curTime;
	int size = adj[cur.dest].size();
	for (int i = 0; i <size; i++) {
		RoadEdge r = adj[cur.dest][i];
		if (visited[r.dest])
			continue;
		visited[r.dest] = true;
		RoadEdge next = { r.dest, r.curTime + cur.curTime, r.minTime + cur.minTime };
		adjustLowHighDFS(next);
		maxCost[cur.dest] = max(maxCost[cur.dest], maxCost[r.dest]);
	}
}

void doable(int goal, CityNode cur){
	int size = adj[cur.dest].size();
	for (int i = 0; i<size; i++) {
		RoadEdge r = adj[cur.dest][i];
		if (visited[r.dest])
			continue;
		visited[r.dest] = true;
		int possibleCut = r.curTime - r.minTime;
		int neededCut = maxCost[r.dest] - goal - cur.moneySpent;
		if (neededCut <= 0)
			continue;
		int actualCut = min(possibleCut, neededCut);
		myMoney -= actualCut;
		if (myMoney<0)
			return;
		CityNode next = { r.dest, actualCut + cur.moneySpent };
		doable(goal, next);
	}
}

int main() {
	scanf("%d%d", &n, &money);
	for (int i = 0; i <= n; i++){
		adj[i].clear();
		maxCost[i] = 0;
		visited[i] = false;
	}
	for (int i = 0; i < n - 1; i++) {
		int start, end, a, b;
		scanf("%d%d%d%d", &start, &end, &a, &b);
		RoadEdge e1 = { end, a, b };
		RoadEdge e2 = { start, a, b };
		adj[start].push_back(e1);
		adj[end].push_back(e2);
	}

	visited[1] = true;
	RoadEdge start = { 1, 0, 0 };
	adjustLowHighDFS(start);

	int mid = 0;
	int ans = high;

	while (low <= high){
		mid = low + (high - low) / 2;
		for (int i = 0; i <= n; i++){
			visited[i] = false;
		}
		visited[1] = true;
		myMoney = money;
		CityNode cityOne = { 1, 0 };
		doable(mid, cityOne);
		if (myMoney >= 0){
			ans = min(ans, mid);
			high = mid - 1;
		}
		else{
			low = mid + 1;
		}
	}

	printf("%d\n", ans);
	return 0;
}