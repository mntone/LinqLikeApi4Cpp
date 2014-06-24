# LinqLikeApi for C++

## License
Header, “linq.hpp,” is “[MIT license](./license.md).”

## Support platform
- Windows (Client/Server & Phone, WinRT Platforms)
- Linux with clang (I’m checking on Debian.)

## How to

### 1. From

	vector<int> vec { 1, 3, 2 };
	auto linq = Linq::From( vec );


### 2. Range

	auto linq = Linq::Range( 1, 3 );


### 3. Repeat

	auto linq = Linq::Repeat( 0, 3 );


## Summary

### Getter
- Begin
- End
- First
- Last
- At

### Conditional Judgement
- All
- Any
- None
- Empty
- SequenceEqual
- Contain/Include

### Basic Calc
- Count
- Sum
- Average/Mean
- GeometricAverage/GeometricMean, not implement!
- Aggregate
- Minimum
- Maximum
- Median
- Variance
- StandardDeviation

### Filtering
- OfType (for WinRT)
- EqualTo
- NotEqualTo
- LessThan
- LessThanOrEqualTo
- GreaterThan
- GreaterThanOrEqualTo
- Where

### Basic Operation
- Skip
- SkipWhile
- Take
- TakeWhile
- Reverse
- Rotate
- OrderBy
- OrderByDescending

### Set Calc, not implement!
- Distinct
- Concat
- Except/Differ
- Union
- Intersect
- SymmetricDiffer
- Zip

### Conversion
- Cast
- Square
- Select

### Vectorlize/Maplize
- to_vector
- to_deque
- to_list
- to_forward_list
- to_map
- to_multimap
- to_unordered_map
- to_unordered_multimap

### Vectorlize/Maplize (for WinRT)
- ToVector
- ToVectorView
- ToDeque
- ToDequeView
- ToMap
- ToMapView
- ToUnorderedMap
- ToUnorderedMapView