#include <vector>
#include <set>

class Solution {
public:
    std::vector<double> medianSlidingWindow(std::vector<int>& nums, int k) {
        std::vector<double> medians;
        std::multiset<long long> lower; // Max-heap equivalent (stores lower half)
        std::multiset<long long> upper; // Min-heap equivalent (stores upper half)

        // Helper function to balance the two multisets
        auto balance = [&]() {
            if (lower.size() > upper.size() + 1) {
                upper.insert(*lower.rbegin());
                lower.erase(std::prev(lower.end()));
            } else if (lower.size() < upper.size()) {
                lower.insert(*upper.begin());
                upper.erase(upper.begin());
            }
        };

        for (int i = 0; i < nums.size(); ++i) {
            // 1. Insert the new element into the appropriate half
            if (lower.empty() || nums[i] <= *lower.rbegin()) {
                lower.insert(nums[i]);
            } else {
                upper.insert(nums[i]);
            }
            balance();

            // 2. Remove the element that has fallen out of the sliding window
            if (i >= k) {
                long long elementToRemove = nums[i - k];
                auto it = lower.find(elementToRemove);
                if (it != lower.end()) {
                    lower.erase(it);
                } else {
                    upper.erase(upper.find(elementToRemove));
                }
                balance();
            }

            // 3. Extract the median once the window reaches size k
            if (i >= k - 1) {
                if (k % 2 == 1) {
                    medians.push_back(static_cast<double>(*lower.rbegin()));
                } else {
                    medians.push_back((*lower.rbegin() + *upper.begin()) / 2.0);
                }
            }
        }

        return medians;
    }
};
