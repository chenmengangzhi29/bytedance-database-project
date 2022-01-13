#include "data/csv_data_loader.h"
#include "table/column_table.h"
#include "table/custom_table.h"
#include "table/indexed_row_table.h"
#include "table/row_table.h"
#include "table/table.h"
#include "test_base.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace bytedance_db_project;

class Lab1Test : public TestBase {};

TEST_F(Lab1Test, TestRowTable) {
  RowTable rt;
  rt.Load(loader);
  rt.PutIntField(9,1,7);
  ASSERT_EQ(7,rt.GetIntField(9,1));
  ASSERT_EQ(-1,rt.GetIntField(20,1));
  ASSERT_EQ(-1,rt.GetIntField(19,-1));
  ASSERT_EQ(68, rt.ColumnSum());
}

TEST_F(Lab1Test, TestColumnTable) {
  ColumnTable rt;
  rt.Load(loader);
  rt.PutIntField(9,1,6);
  ASSERT_EQ(6,rt.GetIntField(9,1));
  ASSERT_EQ(-1,rt.GetIntField(20,1));
  ASSERT_EQ(-1,rt.GetIntField(19,-1));
  ASSERT_EQ(68, rt.ColumnSum());
}