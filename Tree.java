import java.util.*;
import java.util.function.*;
import java.util.stream.Collectors;

/**
 * An implementation of a tree data structure.
 * Each element is allowed to have any arbitrary amount of children.
 * To be built bottom-up by constructing trees out of existing subtrees.
 * 
 * <br>More generally, a tree is a fully-connected acyclic graph,
 * whereby each element has a single parent and some children.
 * 
 * <br>Requires Java SE 1.8 or later.
 * 
 * @author Alec Dorrington
 * 
 * @param <T> the type of value of which the tree is to be composed.
 */
public class Tree<T> implements Iterable<T> {
    
    /**
     * The node at the root of the tree. Will never be null.
     */
    private final TreeNode<T> root;
    
    /**
     * Construct a new single-node tree with the given root value.
     * @param root the value at the root of the tree.
     */
    public Tree(T root) {
        this.root = new TreeNode<>(root);
    }
    
    /**
     * Construct a new tree as a shallow copy of another existing tree.
     * @param tree the tree to copy.
     */
    public Tree(Tree<T> tree) {
        root = tree.clone().root;
    }
    
    /**
     * Construct a new tree with the given root and immediate children.
     * @param root the value at the root of the tree.
     * @param children depth one children of the tree.
     */
    @SafeVarargs
    public Tree(T root, T... children) {
        this.root = new TreeNode<>(root);
        for(T c : children) addChild(c);
    }
    
    /**
     * Construct a new tree with the given root and subtrees.
     * The roots of the subtrees become immediate children of the new root.
     * Subtrees are NOT copied. They continue to refer to parts of this tree.
     * @param root the value at the root of the tree.
     * @param subTrees subtrees to be attached to the root.
     */
    @SafeVarargs
    public Tree(T root, Tree<T>... subTrees) {
        this.root = new TreeNode<>(root);
        for(Tree<T> s : subTrees) addSubTree(s);
    }
    
    /**
     * Construct a tree to act as a shell around a particular node or subtree.
     * @param root the node to act as the root of the tree.
     */
    private Tree(TreeNode<T> root) { this.root = root; }
    
    /**
     * Get the value at the root of the tree.
     * @return the root value.
     */
    public T root() { return root.value; }
    
    /**
     * Replace the value at the root of the tree with a new value.
     * @param root the new root value.
     */
    public void setRoot(T root) { this.root.value = root; }
    
    /**
     * Get a list of all subtrees attached to the root.
     * These are NOT copied, and continue to refer to parts of this tree.
     * @return list of subtrees.
     */
    public List<Tree<T>> subTrees() {
        return root.children.stream().map(Tree<T>::new)
                .collect(Collectors.toList());
    }
    
    /**
     * Add a new node as an immediate child of the root.
     * The new node will be placed after any existing children.
     * @param child a depth one child.
     */
    public void addChild(T child) {
        root.children.add(new TreeNode<>(child));
    }
    
    /**
     * Add a new node as an immediate child of the root.
     * The new node will be inserted amongst its siblings as per the index.
     * The given index must be between 0 and the number of children inclusive.
     * @param index the index at which to insert the new child.
     * @param child a depth one child.
     * @throws IndexOutOfBoundsException
     */
    public void addChild(int index, T child) {
        try {
            root.children.add(index, new TreeNode<>(child));
        } catch(IndexOutOfBoundsException e) {
            throw new IndexOutOfBoundsException(e.getMessage());
        }
    }
    
    /**
     * Add a new subtree as an immediate child of the root.
     * The root of the subtree becomes an immediate child of the main root.
     * The subtree is NOT copied. It continues to refer to part of this tree.
     * @param subTree subtree to attach to the root.
     */
    public void addSubTree(Tree<T> subTree) {
        root.children.add(subTree.root);
    }
    
    /**
     * Add a new subtree as an immediate child of the root.
     * The root of the subtree becomes an immediate child of the main root.
     * The new subtree will be inserted amongst its siblings as per the index.
     * The given index must be between 0 and the number of children inclusive.
     * @param index the index at which to insert the new subtree.
     * @param subTree subtree to attach to the root.
     * @throws IndexOutOfBoundsException
     */
    public void addSubTree(int index, Tree<T> subTree) {
        try {
            root.children.add(index, subTree.root);
        } catch(IndexOutOfBoundsException e) {
            throw new IndexOutOfBoundsException(e.getMessage());
        }
    }
    
    /**
     * Remove all, if any, elements matching a predicate from the tree.
     * When an element is removed, its children are promoted to take its place,
     * rather than them being removed also.
     * This function modifies THIS version of the tree.
     * @param selector the function by which elements are selected for removal.
     */
    public void removeIf(Predicate<T> selector) {
        removeIfRecurse(selector, root);
    }
    
    /**
     * The recursive part of 'removeIf' (refer therein for more details).
     * @param selector the function by which elements are selected for removal.
     * @param tracker the current element being considered.
     */
    private void removeIfRecurse(Predicate<T> selector, TreeNode<T> tracker) {
        
        //Remove all matching descendants of this node.
        tracker.children.forEach(n -> removeIfRecurse(selector, n));
        List<TreeNode<T>> children = new LinkedList<>();
        
        //Remove this node if it matches, promoting children as necessary.
        tracker.children.forEach(n -> {
            if(selector.test(n.value)) children.addAll(n.children);
            else children.add(n);
        });
        tracker.children = children;
    }
    
    /**
     * Determines if the tree contains any elements matching a predicate.
     * @param selector the predicate to be met by any elements.
     * @return true if any matching elements are found, false otherwise.
     */
    public boolean contains(Predicate<T> selector) {
        return containsRecurse(selector, root);
    }
    
    /**
     * The recursive part of 'contains' (refer therein for more details).
     * @param selector the predicate to be met by any elements.
     * @param tracker the current element being considered.
     * @return whether this element or any of its children match.
     */
    private boolean containsRecurse(Predicate<T> selector,
            TreeNode<T> tracker) {
        
        //Return true if this node matches the predicate.
        if(selector.test(tracker.value)) return true;
        
        //Return true if any descendants of this node match the predicate.
        for(TreeNode<T> child : tracker.children) {
            if(containsRecurse(selector, child)) return true;
        }
        return false;
    }
    
    /**
     * Maps each value to a different value as per a mapping function.
     * Maintains the overall structure of the tree.
     * Produces a new tree without modifying this existing one.
     * @param mapper the function by which to map elements.
     * @return a new tree with accordingly mapped elements.
     */
    public <U> Tree<U> map(Function<T, U> mapper) {
        return new Tree<>(mapRecurse(mapper, root));
    }
    
    /**
     * The recursive part of 'map' (refer therein for more details).
     * @param mapper the function by which to map elements.
     * @param tracker the current element being considered.
     * @return a new subtree with accordingly mapped elements.
     */
    private <U> TreeNode<U> mapRecurse(Function<T, U> mapper,
            TreeNode<T> tracker) {
        
        //Map this node onto the new tree.
        U value = mapper.apply(tracker.value);
        TreeNode<U> node = new TreeNode<>(value);
        
        //Map all descendants of this node onto the new tree.
        tracker.children.forEach(n -> node.children.add(
                mapRecurse(mapper, n)));
        return node;
    }
    
    /**
     * Produces a list containing every element in the tree.
     * Elements are presented in depth first search order.
     * @return the list of values in the tree.
     */
    public List<T> toList() {
        List<T> list = new LinkedList<>();
        toListRecurse(root, list);
        return list;
    }
    
    /**
     * The recursive part of 'toList' (refer therein for more details).
     * @param tracker the current element being considered.
     * @param list the list of elements in the tree.
     */
    private void toListRecurse(TreeNode<T> tracker, List<T> list) {
        list.add(tracker.value);
        tracker.children.forEach(n -> toListRecurse(n, list));
    }
    
    /**
     * Produces a shallow copy of the tree.
     * @return a copy of the tree.
     */
    public Tree<T> clone() {
        return new Tree<>(cloneRecurse(root));
    }
    
    /**
     * The recursive part of 'clone' (refer therein for more details).
     * @param tracker the current element being considered.
     * @return a copy the subtree.
     */
    private TreeNode<T> cloneRecurse(TreeNode<T> tracker) {
        
        TreeNode<T> node = new TreeNode<>(tracker.value);
        tracker.children.forEach(n -> node.children.add(
                cloneRecurse(n)));
        return node;
    }
    
    /**
     * Provides an iterator which iterates over every element in the tree,
     * in depth first search order.
     * @return an iterator as described above.
     */
    @Override
    public Iterator<T> iterator() {
        return toList().iterator();
    }
    
    /**
     * Provides a String representation of the tree, whereby each element
     * occupies its own line, also featuring appropriate indentation.
     * @return a String representation of the tree.
     */
    @Override
    public String toString() {
        return toStringRecurse(root, "");
    }
    
    /**
     * The recursive part of 'toString' (refer therein for more details).
     * @param tracker the current element being considered.
     * @param indent the prefix to be applied to start of each line.
     * @return a string representing this particular subtree.
     */
    private String toStringRecurse(TreeNode<T> tracker, String indent) {
        
        String str = indent + tracker.value;
        for(TreeNode<T> child : tracker.children) {
            str += '\n' + toStringRecurse(child, indent + "   ");
        }
        return str;
    }
    
    /**
     * Determines if the tree is equivalent to another tree.
     * This only applies if both trees have identical structure and values.
     * Equivalence of values is determined by '.equals()'.
     * @param o the object to check for equivalence.
     * @return true if the trees are equivalent, false otherwise.
     */
    @Override
    public boolean equals(Object o) {
        return o instanceof TreeNode && equalsRecurse(
                root, ((Tree<?>) o).root);
    }
    
    /**
     * The recursive part of 'equals' (refer therein for more details).
     * @param tracker1 the current element being considered on this tree.
     * @param tracker2 the current element being considered on the other tree.
     * @return whether these two particular subtrees are equal.
     */
    private boolean equalsRecurse(TreeNode<T> tracker1, TreeNode<?> tracker2) {
        
        //Return false if these particular nodes aren't equivalent.
        if(!tracker1.value.equals(tracker2.value)
                || tracker1.children.size() != tracker2.children.size()) {
            return false;
        }
        
        //Return false if any descendants of these nodes aren't equivalent.
        for(int i = 0; i < tracker1.children.size(); i++) {
            if(!equalsRecurse(tracker1.children.get(i),
                    tracker2.children.get(i))) {
                return false;
            }
        }
        return true;
    }
    
    /**
     * Represents a single given element in a tree.
     * @param <T> the type of value stored by this node, and its tree.
     */
    protected static class TreeNode<T> {
        
        /** The value stored at this position in the tree. */
        T value;
        
        /** An ordered list of the children of this node. */
        List<TreeNode<T>> children = new LinkedList<>();
        
        /**
         * Construct a new node with a particular value.
         * @param value the value stored in this node.
         */
        TreeNode(T value) { this.value = value; }
    }
}
