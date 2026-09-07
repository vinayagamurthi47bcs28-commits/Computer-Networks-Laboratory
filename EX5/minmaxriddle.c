def riddle(arr):
    n = len(arr)
    left = [-1] * n
    right = [n] * n
    stack = []
    
    # 1. Find the first smaller element to the left
    for i in range(n):
        while stack and arr[stack[-1]] >= arr[i]:
            stack.pop()
        if stack:
            left[i] = stack[-1]
        stack.append(i)
        
    stack = [] # Reset stack
    
    # 2. Find the first smaller element to the right
    for i in range(n - 1, -1, -1):
        while stack and arr[stack[-1]] >= arr[i]:
            stack.pop()
        if stack:
            right[i] = stack[-1]
        stack.append(i)
        
    # 3. Map window sizes to their max minimum values
    # result[x] will temporarily hold the maximum element for an exact window size x
    result = [0] * (n + 1)
    for i in range(n):
        window_size = right[i] - left[i] - 1
        result[window_size] = max(result[window_size], arr[i])
        
    # 4. Inverted loop to fill missing window sizes
    # If a value is the min of a window of size k, it's also a valid min for size k-1
    for i in range(n - 1, 0, -1):
        result[i] = max(result[i], result[i + 1])
        
    # Return 1-indexed window sizes from 1 to n
    return result[1:]
