import matplotlib.pyplot as plt
import sys

def process_test_results(filename):
    test_data = {}
    with open(filename, 'r') as file:
        for line in file:
            parts = line.strip().split()
            if len(parts) < 5:  
                continue        # //! skip error test
        
            test_num = int(parts[1]) 
            time     = float(parts[4])    

            if test_num not in test_data:
                test_data[test_num] = []
            test_data[test_num].append(time)
            
    results = []
    for test_num in sorted(test_data.keys()):
        times = test_data[test_num]
        mean  = sum(times) / len(times)
        n     = len(times)

        sum_sq_diff = sum((x - mean) ** 2 for x in times)
        variance    = sum_sq_diff / (n - 1)
        stdev       = variance ** 0.5
        results.append((test_num, mean, stdev))
    
    return results

def least_squares_method(x_points, y_points):
    n = len(x_points)
    sum_x = sum_y = sum_xy = sum_x2 = 0
    for x, y in zip(x_points, y_points):
        sum_x  += x
        sum_y  += y
        sum_xy += x * y
        sum_x2 += x * x
    d = n * sum_x2 - sum_x ** 2

    a = (n * sum_xy - sum_x * sum_y) / d
    b = (sum_y - a * sum_x) / n
    
    return a, b

def draw_results(slow_results, fast_results, output_file="test.svg"):
    slow_x = [r[0] for r in slow_results]
    slow_y = [r[1] for r in slow_results]
    fast_x = [r[0] for r in fast_results]
    fast_y = [r[1] for r in fast_results]
    
    a_slow, b_slow = least_squares_method(slow_x, slow_y)
    a_fast, b_fast = least_squares_method(fast_x, fast_y)

    x_min = min(slow_x)
    x_max = max(fast_x)
    
    reg_slow_x = [x_min, x_max]
    reg_slow_y = [a_slow * x_min + b_slow, a_slow * x_max + b_slow]
    
    reg_fast_x = [x_min, x_max]
    reg_fast_y = [a_fast * x_min + b_fast, a_fast * x_max + b_fast]
    
    plt.figure(figsize=(14,7))
    
    slow_plot = plt.errorbar(slow_x, slow_y, yerr=[r[2] for r in slow_results], 
                 fmt='o', color='red', label='Медленная реализация', 
                 capsize=3, markersize=6, markeredgewidth=1)
    
    fast_plot = plt.errorbar(fast_x, fast_y, yerr=[r[2] for r in fast_results], 
                 fmt='o', color='green', label='Быстрая реализация', 
                 capsize=3, markersize=6, markeredgewidth=1)
    
    plt.plot(reg_slow_x, reg_slow_y, '-', color='blue', linewidth=2)
    plt.plot(reg_fast_x, reg_fast_y, '-', color='blue', linewidth=2)
    
    plt.legend(handles=[
        plt.Line2D([0], [0], 
                  marker=slow_plot.lines[0].get_marker(),
                  color='w',
                  markerfacecolor=slow_plot.lines[0].get_color(),
                  markersize=12,
                  label='Медленная реализация'),
        
        plt.Line2D([0], [0],
                  marker=fast_plot.lines[0].get_marker(),
                  color='w',
                  markerfacecolor=fast_plot.lines[0].get_color(),
                  markersize=12,
                  label='Быстрая реализация')
    ])
    
    plt.xlabel('Номер теста')
    plt.ylabel('Число тактов')
    plt.title('Сравнение быстрой и медленной реализаций множества Мандельброта')
    plt.grid(True, linestyle='--', alpha=0.7)
    
    plt.xlim(x_min - 1, x_max + 1)
    plt.xticks(range(0, x_max + 1, 5))
    
    plt.savefig(output_file, format='svg', bbox_inches='tight')
    print(f"График сохранён в {output_file}")

slow_results = process_test_results(sys.argv[1])
fast_results = process_test_results(sys.argv[2])
draw_results(slow_results, fast_results)
