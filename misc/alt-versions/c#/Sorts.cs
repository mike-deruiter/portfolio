using System;
using System.Collections.Generic;

namespace MikeCSharp
{
    public static class Sorts
    {
        public static void Quicksort(IComparable[] arr) 
        {
            int i, j, l, r;
            int lo, mid, hi;
            IComparable prt, tmp;
            Stack<int> s;

            l = 0; r = arr.Length-1; s = new Stack<int>();

            //Non-Recursive
            while (true) {
                while (r > l) {
                    //Median-of-Three Partitioning
                    lo = l; hi = r; mid = (l + r) / 2;

                    if (arr[mid].CompareTo(arr[lo]) < 0) {
                        tmp = arr[mid]; arr[mid] = arr[lo]; arr[lo] = tmp;
                    }

                    if (arr[hi].CompareTo(arr[lo]) < 0) {
                        tmp = arr[hi]; arr[hi] = arr[lo]; arr[lo] = tmp;
                    }

                    if (arr[hi].CompareTo(arr[mid]) < 0) {
                        tmp = arr[hi]; arr[hi] = arr[mid]; arr[mid] = tmp;
                    }

                    tmp = arr[mid]; arr[mid] = arr[hi]; arr[hi] = tmp;

                    //Quicksort
                    prt = arr[hi]; i = l; j = r;

                    while (true) {
                        while (arr[++i].CompareTo(prt) < 0) ;
                        while (arr[--j].CompareTo(prt) > 0) ;

                        if (j < i)
                            break;

                        tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
                    }

                    tmp = arr[i]; arr[i] = arr[r]; arr[r] = tmp;

                    if (i - 1 > r - i) {
                        s.Push(l); s.Push(i - 1); l = i + 1;
                    }
                    else {
                        s.Push(i + 1); s.Push(r); r = i - 1;
                    }
                } //end while (r > l)

                if (s.Count == 0) {
                    break;
                }

                r = s.Pop(); l = s.Pop();               
            } //end while (true)
        } //end Quicksort()
    } //end Sorts
} //end MikeCSharp