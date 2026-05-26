list :: [Int] -> String
list [] = "List is empty."
list [x] = "List with one element: " ++ show x
list [a, b] = "List with two elements: " ++ show a ++ ", " ++ show b
list _ = "List with many elements."

sumList :: [Int] -> Int
jumList [] = 0
sumList (x:xs) = x + sumList xs

main = do
    print $ list [1,2]
    print (sumList [1, 2, 3, 4, 5])
