{-# LANGUAGE BangPatterns #-}

module Secret where
import System.IO.Unsafe

data World = World deriving Show

printStr :: String -> World -> World
printStr s !w = unsafePerformIO (putStrLn s >> return w)

readStr :: World -> (String, World) 
readStr !w = unsafePerformIO (getLine >>= (\s -> return (s, w)))
