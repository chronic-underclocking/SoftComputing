#include <math.h>

namespace fl
{
    /*
     ********************
     *   Point struct   *
     ********************
    */

    struct point
    {
        float x;
        float y;
    };

    /*
     ************
     *   Sets   *
     ************
    */

    point project_funding[4][4] = {
                                    {{0.0, 0.0}, {0.0, 1.0}, {10.0, 1.0}, {30.0, 0.0}},   // Very low
                                    {{10.0, 0.0}, {30.0, 1.0}, {40.0, 1.0}, {60.0, 0.0}}, // Low
                                    {{40.0, 0.0}, {60.0, 1.0}, {70.0, 1.0}, {90.0, 0.0}}, // Medium
                                    {{70.0, 0.0}, {90.0, 1.0}, {100.0, 1.0}, {100, 0.0}}  // High
                                  };

    point team_experience_level[3][3] = {
                                            {{0.0, 0.0}, {15.0, 1.0}, {30.0, 0.0}},  // Beginner
                                            {{15.0, 0.0}, {30.0, 1.0}, {45.0, 0.0}}, // Intermediate
                                            {{30.0, 0.0}, {60.0, 1.0}, {60.0, 0.0}}  // Expert
                                        };

    point risk[3][3] = {
                            {{0.0, 0.0}, {25.0, 1.0}, {50.0, 0.0}},   // Low
                            {{25.0, 0.0}, {50.0, 1.0}, {75.0, 0.0}},  // Normal
                            {{50.0, 0.0}, {100.0, 1.0}, {100.0, 0.0}} // High
                        };

    /*
     *********************
     * Membership Values *
     *********************
    */

   float pf_very_low;
   float pf_low;
   float pf_medium;
   float pf_high;
   float tel_beginner;
   float tel_intermediate;
   float tel_expert;
   float risk_low;
   float risk_normal;
   float risk_high;

   /*
     *************
     * Centroids *
     *************
    */

   float c_low;
   float c_normal;
   float c_high;

   /*
     ******************
     * Line Equations *
     ******************
    */

   inline float getSlope(point p1, point p2)
   {
       return ((p2.y - p1.y) / (p2.x - p1.x));
   }

   inline float getIntercept (float slope, point p1)
   {
       return (p1.y - (slope * p1.x));
   }

   inline float getMembership(float value, point arr[], int arrSize)
   {
       float slope;
       float intercept;
       for(int i = 0; i < arrSize-1; i++)
       {
           if(value >= arr[i].x && value <= arr[i+1].x)
           {
               slope = getSlope(arr[i],arr[i+1]);
               intercept = getIntercept(slope, arr[i]);
               return ((slope * value) + intercept);
           }
       }
       return 0;
   }

   inline float getCentroid(point arr[], int arrSize)
   {
       float result = 0;
       for(int i = 0; i < arrSize; i++)
       {
           result += arr[i].x;
       }
       return (result / arrSize);
   }

   /*
     ********************
     * Utility Function *
     ********************
    */

   inline bool compareFloatsEqual(float a, float b)
   {
       return std::fabs(a - b) < 1e-06;
   }
}