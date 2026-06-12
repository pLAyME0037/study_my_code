def firstn(n):
    i = 0
    while i < n:
        yield i
        i += 1

# print(list(firstn(10)))
# print(dir(firstn(10)))

g = firstn(10)
print(g.__next__())
print(g.__next__())
