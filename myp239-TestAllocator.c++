// -------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// -------------------------------------

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator

#include "gtest/gtest.h"

#include "Allocator.h"

// --------------
// TestAllocator1
// --------------

template <typename A>
struct TestAllocator1 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            std::allocator<int>,
            std::allocator<double>,
            my_allocator<int,    100>,
            my_allocator<double, 100>>
        my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);

TYPED_TEST(TestAllocator1, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 1;
    const value_type     v = 2;
    const pointer        p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator1, test_10) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type  x;
    const size_type       s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}

// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
    const my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, index) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

// --------------
// TestAllocator3
// --------------

template <typename A>
struct TestAllocator3 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            my_allocator<int,    100>,
            my_allocator<double, 100>>
        my_types_2;

TYPED_TEST_CASE(TestAllocator3, my_types_2);

TYPED_TEST(TestAllocator3, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 1;
    const value_type     v = 2;
    const pointer        p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator3, test_10) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 10;
    const value_type     v = 2;
    const pointer        b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}



TEST(TestAllocator4, size_enough) {
  try {
    my_allocator<double, 8> x;
    ASSERT_EQ(1, 0);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator5, checksizeofn) {
  try {
    my_allocator<int, 200> x;
    x.allocate(201);
    ASSERT_EQ(1, 0);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 1);
  }
}


TEST(TestAllocator5, match) {
  try {
    my_allocator<int, 500> x;
    x.allocate(8);
    ASSERT_EQ(1, 1);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 0);
  }
}


TEST(TestAllocator5, fill) {
  try {
    my_allocator<int, 100> x;
    x.allocate(4);
    x.allocate(3);
    x.allocate(1);
    ASSERT_EQ(1, 1);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 0);
  }
}

TEST(TestAllocator5, split) {
  try {
    my_allocator<int, 40> x;
    x.allocate(4);
    ASSERT_EQ(1, 1);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 0);
  }
}

// ---------------------------------
// TestAllocator6 (deallocate tests)
// ---------------------------------

template <typename A> struct TestAllocator6 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>> my_types_3;

TYPED_TEST_CASE(TestAllocator6, my_types_3);

// test for nullptr checking
TYPED_TEST(TestAllocator6, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 2;
  const value_type v = 3;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.construct(p, v);
    ASSERT_EQ(v, *p);
    x.destroy(p);
    try {
      x.deallocate(nullptr, s);
      ASSERT_EQ(1, 0);
    } catch (std::invalid_argument &e) {
      x.deallocate(p, s);
      ASSERT_EQ(1, 1);
    }
  } else {
    ASSERT_EQ(1, 0);
  }
}

// test for p < a pointer error
TYPED_TEST(TestAllocator6, test_2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 2;
  const value_type v = 3;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.construct(p, v);
    ASSERT_EQ(v, *p);
    x.destroy(p);
    try {
      x.deallocate(p - 2, s);
      ASSERT_EQ(1, 0);
    } catch (std::invalid_argument &e) {
      x.deallocate(p, s);
      ASSERT_EQ(1, 1);
    }
  } else {
    ASSERT_EQ(1, 0);
  }
}



// test whether deallocate frees a single block only
TYPED_TEST(TestAllocator6, test_6) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 4;
  const pointer p = x.allocate(2);
  if (p != nullptr) {
    try {
      x.deallocate(p, s);
      x.allocate(2);
      ASSERT_EQ(1, 1);
    } catch (std::invalid_argument &e) {
      x.deallocate(p, s);
      ASSERT_EQ(1, 0);
    }
  } else {
    ASSERT_EQ(1, 0);
  }
}
