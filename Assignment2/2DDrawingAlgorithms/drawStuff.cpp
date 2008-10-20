
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "drawStuff.h"
#include "setPixel.h"

using namespace std;

void drawLine(int x0, int y0, int x1, int y1) {  

	int dX; 
	int dY; 
	int dA; 
	int dB; 
	int d; 
	int x; 
	int y; 

	// Swap points
	if (x1 < x0) { 
		int t; 
		t = x1; 
		x1 = x0; 
		x0 = t; 
		t = y1; 
		y1 = y0; 
		y0 = t; 
	} 

	dX = x1 - x0; 
	dY = y1 - y0; 

	x = x0; 
	y = y0; 

	if ((dY >= 0) && (dY <= dX)) { 

		// Slope is 0 or positive and shallow or 1

		d = 2 * dY - dX;
		dA = 2 * dY;
		dB = 2 * (dY - dX);
 
		setPixel(x, y); 

		while (x < x1) { 
			if (d <= 0) { 
				d += dA; 
				++x; 
			} else { 
				d += dB; 
				++x; 
				++y; 
			} 
			setPixel(x, y); 
		} 
	} else if((dY < 0) && (0 - dY <= dX) && (dX > 0)) {

		// Slope is negative and shallow or -1

		d = 2 * dY + dX;
		dA = 2 * dY;
		dB = 2 * (dY + dX);

		x = x0;
		y = y0;

		setPixel(x, y); 

		while(x < x1) {
			if(d <= 0) {
				d += dB; 
				++x; 
				--y; 
			} else {
				d += dA; 
				++x; 
			} 
			setPixel(x, y); 
		} 
	} else if((dY > 0) && (dY > dX)){ 

		// Slope is positive and steep

		d = 2 * dX - dY;
		dA = 2 * dX;
		dB = 2 * (dX - dY);

		setPixel(x, y); 

		while(y < y1) {
			if(d <= 0) {
				d += dA; 
				++y; 
			} else {
				d += dB; 
				++y; 
				++x; 
			} 
			setPixel(x, y); 
		} 
	} else if((dY < 0) && (0 - dY > dX) ) {

		// Slope is negative and steep

		d = 2 * dX + dY;
		dA = 2 * dX;
		dB = 2 * (dY + dX);

		setPixel(x, y); 

		while(y > y1) {
			if(d <= 0) {
				d += dA; 
				--y; 
			} else {
				d += dB; 
				--y; 
				++x; 
			} 
			setPixel(x, y); 
		} 
	} 
} 

struct Edge {
	//The minimum y value of the two vertices.
	int minY;
	//The maximum y value of the two vertices.
	int maxY;
	//The x value associated with the minimum y value.
	double x;
	//The slope of the edge.
	double slope;
	
	bool operator<(const Edge& compareTo) {
		return minY < compareTo.minY || ( minY == compareTo.minY && x < compareTo.x );
	}

};

struct ActiveEdge {

	ActiveEdge(const Edge& e) {
		minY = e.minY;
		maxY = e.maxY;
		x = e.x;
		slope = e.slope;
	}

	//The minimum y value of the two vertices.
	int minY;
	//The maximum y value of the two vertices.
	int maxY;
	//The x value associated with the minimum y value.
	double x;
	//The slope of the edge.
	double slope;

	bool operator<(const ActiveEdge& compareTo) {
		return x < compareTo.x;
	}
};

// Draw a filled polygon. The polygon has n distinct vertices. 
// The coordinates of the vertices making up the polygon are stored in the x and y arrays.  
// The ith vertex will have coordinate  (x[i], y[i])
void drawPolygon(int n, int x[], int y[]) {
	
	// 1 - Create edge table

	vector<Edge> all_edges;

	unsigned int i;
	for(i = 0; i < (unsigned int)n; ++i) {

		Edge edge = *new Edge();

		int nextI = (i + 1) % n;
		
		// Find min y, max y, x of min y for current edge
		if(y[i] <= y[nextI]) {
			edge.minY = y[i];
			edge.maxY = y[nextI];
			edge.x = x[i];
		} else {
			edge.maxY = y[i];
			edge.minY = y[nextI];
			edge.x = x[nextI];
		}

		int maxX;
		int minX;
		if(x[i] <= x[nextI]) {
			minX = x[i];
			maxX = x[nextI];
		} else {
			minX = x[nextI];
			maxX = x[i];
		}
		
		// Find slope of edge
		//double slope = (edge.maxY - edge.minY) / ((float)maxX - minX);
		double slope = (y[i] - y[nextI]) / (float)(x[i] - x[nextI]);
		if(slope != 0) {
			edge.slope = 1 / slope;
			all_edges.push_back(edge);
		}
	}

	// 2 - sort edge table ascending (y, x)

	sort(all_edges.begin(), all_edges.end());

	// 3 - Initialize

	unsigned int p = 0;
	int scanY = all_edges[0].minY;

	// Active Edge Table
	vector<ActiveEdge> aet;

	while(all_edges.size() > 0 && all_edges[0].minY <= scanY) {
		aet.push_back(*new ActiveEdge(all_edges[0]));
		all_edges.erase(all_edges.begin());
	}
	
	// 4 - Fill Polygon
	
	while(aet.size() > 0) {
		//1. Draw all pixels from the x value of odd to the x value of even parity edge pairs.
		for(p = 0; p + 1 < aet.size(); p+=2) {
			//drawLine((int)aet[p].x, scanY, (int)aet[p + 1].x, scanY);
			for(int x = (int)aet[p].x; x < (int)aet[p + 1].x; ++x) {
				setPixel(x, scanY);
			}
		}

		//2. Increase the scan-line by 1.
		++scanY;

		//3. Remove any edges from the active edge table for which the maximum y value is equal to the scan_line.
		for(i = 0; i < aet.size();) {
			if(aet[i].maxY == scanY) {
				aet.erase(aet.begin() + i);
			} else {
				++i;
			}
		}

		//4. Update the x value for for each edge in the active edge table using the formula x1 = x0 + 1/m.
		for(i = 0; i < aet.size(); ++i) {
			aet[i].x += (aet[i].slope);
		}

		//5. Remove any edges from the global edge table for which the minimum y value is equal to the 
		//   scan-line and place them in the active edge table. 
		for(i = 0; i < all_edges.size();) {
			if(all_edges[i].minY == scanY) {
				aet.push_back(*new ActiveEdge(all_edges[i]));
				all_edges.erase(all_edges.begin() + i);
			} else {
				++i;
			}
		}

		//6. Reorder the edges in the active edge table according to increasing x value. 
		sort(aet.begin(), aet.end());
	}
}

//
/*bool Inside(int x, int y, int clipX0, int clipY0, int clipX1, int clipY1) {
	return true;
}*/

// Finds the intersection point of a line segment and a clip plane and returns that value.
void ComputeIntersection(int aX, int aY, int bX, int bY, int clipX0, int clipY0, int clipX1, int clipY1, int *outX, int *outY) {

	double m = (aY - bY) / ((double)aX - bX);

	// y - y0 = m(x - x0)	

	if(aX > clipX1) {
		//Right
		*outX = clipX1;
		// y = m(x - x0) + y0 
		*outY = (int)(m * (*outX - aX) + aY);
	}

	if(aY > clipY1) {
		//Top
		*outY = clipY1;
		// x = (y - y0) / m + x0;
		*outX = (int)((*outY - aY) / m) + aX;
	}

	if(aX < clipX0) {
		//Left
		*outX = clipX0; 
		// y0 = y - m(x - x0)
		*outY = (int)(aY - m * (*outX - aX));
	}

	if(aY < clipY0) {
		//Bottom
		*outY = clipY0;
		// x0 = x - (y - y0) / m
		*outX = (int)(aX - (*outY - aY) / m);
	}
}

// Clip the polygon with vertex count in and vertices inx/iny against the rectangular clipping 
// region specified by lower-left corner (x0,y0) and upper-right corner (x1,y1). 
// The resulting vertices are placed in outx/outy, with the vertex count placed in out.
void clipPolygon(int in, int inx[], int iny[], int *out, int outx[], int outy[], int x0, int y0, int x1, int y1) {
	
	int x, y;
	int prevX = inx[in - 1];
	int prevY = iny[in - 1];

	int outX = 0, outY = 0;

	vector<int> outx_vec;
	vector<int> outy_vec;

	for(int i = 0; i < in; ++i) {
		
		x = inx[i];
		y = iny[i];

		// if current pt is inside clipping region
		if(x >= x0 && x <= x1 && y >= y0 && y <= y1) {
	
			// if last pt is outside clipping region
			if(prevX < x0 || prevX > x1 || prevY < y0 || prevY > y1) {
				
				ComputeIntersection(prevX, prevY, x, y, x0, y0, x1, y1, &outX, &outY);
				
				//add intersection to out
				outx_vec.push_back(outX);
				outy_vec.push_back(outY);
			}

			//add current pt to out
			outx_vec.push_back(x);
			outy_vec.push_back(y);
	
		// if last pt is inside clipping region
		} else if(prevX >= x0 && prevX <= x1 && prevY >= y0 && prevY <= y1) {
			
			ComputeIntersection(x, y, prevX, prevY, x0, y0, x1, y1, &outX, &outY);

			//add intersection to out
			outx_vec.push_back(outX);
			outy_vec.push_back(outY);
		}

		prevX = x;
		prevY = y;
	}
	
	*out = outx_vec.size();

	for(int i = 0; i < *out; ++i) {
		outx[i] = outx_vec[i];
		outy[i] = outy_vec[i];
	}
}
