using System;
using System.Windows.Forms;
using System.Collections;
namespace TreeViewBound
{
	/// <summary>
	/// Summary description for TreeNodeBound.
	/// </summary>
	public class TreeNodeBound : TreeNode
	{

		private object _value;
		public object Value
		{
			get
			{
				return _value;
			}
			set
			{
				_value = value;
			}
		}

		private object _parentValue;
		public object ParentValue
		{
			get
			{
				return _parentValue;
			}
			set
			{
				_parentValue = value;
			}
		}

		public TreeNodeBound(string name) : base(name)
		{
					
		}

		#region Sorting
		public void SortChilds()
		{
			TreeNode[] nodes = (TreeNode[]) System.Collections.ArrayList.Adapter(this.Nodes).ToArray(typeof(TreeNode));
			Array.Sort(nodes, new TreeNodeComparer());
			this.Nodes.Clear();
			this.Nodes.AddRange(nodes);
		}
		#endregion
	}

	#region TreeNodeComparer (for sorting only
	internal class TreeNodeComparer : object, IComparer
	{
		public int Compare(object x, object y)
		{
			//int resultado = 0;
			TreeNode xNode = (TreeNode) x;
			TreeNode yNode = (TreeNode) y;
			return xNode.Text.CompareTo(yNode.Text);
		}
	}
	#endregion
}
