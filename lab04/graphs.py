import matplotlib.pyplot as plt


def find_average(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
        averages = []
        for line in lines:
            numbers = [int(num) for num in line.split()]
            average = sum(numbers) / len(numbers)
            averages.append(average)
        return averages


bits = [4, 6, 8, 10, 12]

filename1 = 'C:\\code\suaiCrypto2\\build\\lab04\\data\\countN3.txt'
filename2 = 'C:\\code\suaiCrypto2\\build\\lab04\\data\\countS3.txt'
averages1 = find_average(filename1)
averages2 = find_average(filename2)

# for i, avg in enumerate(averages, start=1):
#     print(f'Среднее значение для строки {i}: {avg}')

plt.plot(bits, averages1, marker='o', linestyle='-')

print(averages1)
print(averages2)

# Добавление заголовка и меток осей
plt.title('Второй прообраз')
plt.xlabel('bits')
plt.ylabel('value')

plt.grid(True)
plt.show()

plt.plot(bits, averages2, marker='o', linestyle='-')
plt.title('Коллизия')
plt.xlabel('bits')
plt.ylabel('value')

plt.grid(True)
plt.show()
